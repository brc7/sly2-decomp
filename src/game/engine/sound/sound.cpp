// sound.cpp -- sounds (SND, and BANKSND / STREAMSND) and the sound manager (SNDMGR, the global g_sndmgr at
// D_003DD040): sound flags, pan / pitch / volume, the dirty list, bank and stream sounds (start, pause, refcount,
// pools), stream RPCs, the manager's playback mode and counters. TU 0x1FAF70..0x1FF3B8 (103 functions with the
// linkonce copies at 0x259xxx, address order).
// Entry points: StartupSound (boot), LoadSoundSectionFromBrx / PostSoundLoad / ResetSndmgr (level load and teardown),
// UpdateSndmgr (per frame), CreateSfxSound and the SetSnd* / StopSnd setters (game code), PushMusic / StopMusic,
// StartDialog / StopDialog, and the music-layer request counters.
//
// Types: TU-private SND / BANKSND / SNDMGR views for the m2c bodies (sly2/aseg.h's SND is a 3-word stub).
// g_sndmgr is defined here: SNDMGR_ctor constructs it, and its destructor releases the counted sound pointers at
// +0x200, +0x208[8] and +0x228 (slot 2 of each object's vtable), then runs ~BASIC (_$_5BASIC, in-charge flag 2).
// Spelled as a base with that destructor and the pointers as members with an inline release, g++ generates the
// 123-instruction construct-and-destruct StaticInit_sound and the _GLOBAL_$D$ stub GlobalDestructors_sound.
// The hand-written per-function files keep their views in namespaces named after each function (as jt.cpp does).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_sound
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_sound
// SLY2_ALIAS: _GLOBAL_$D$*=GlobalDestructors_sound

#include "sly2/basic.h"
#include "sly2/slotheap.h"
#include "sly2/cm.h"
#include "sly2/lo.h"
#include "sly2/sndmgr.h"
#include "sly2/step.h"
#include "sly2/vu0.h"
#include "sly2/sw.h"
#include "sly2/clock.h"
#include "sly2/vtables.h"
#include "bis.h"
#include "sly2/snd.h"

/* g_sndmgr's static construction and destruction. */
struct SNDOBJ { char pad0[0xC]; VTE *vtable; };
struct SNDMGRG;
extern "C" {
void SNDMGR_ctor(SNDMGRG *soundManager);
}
/* The pads follow SNDMGR's layout (sly2/sndmgr.h), so the fields below sit where SNDMGR has them on any target. */
struct SNDMGRBASE
{
    char ab[SLY2_OFFSETOF(SNDMGR, dirtySounds)];
    DL dlSndDirty;          /* +0x4C [S] draft name: the sounds RemoveSndFromDirtyList takes out (was the address name D_003DD08C) */
    char ab58[SLY2_OFFSETOF(SNDMGR, dialogSound) - SLY2_OFFSETOF(SNDMGR, isPaused)];
    // Destroy the object (~BASIC, in-charge flag 2).
    ~SNDMGRBASE() { ((BASIC *)this)->BASIC::~BASIC(); }
};
struct SNDMGRG : public SNDMGRBASE
{
    SNDREF dialogSound;      /* +0x200 */
    int musicStackCount;     /* +0x204 */
    SNDREF musicStack[8];    /* +0x208 */
    SNDREF ambienceSound;    /* +0x228 */
    /* The dialog state at the end of SNDMGR. g_sndmgr is the whole 0x250-byte SNDMGR: ApplyDialogEvent, UpdateDialog,
       StartDialog and StopDialog read these words through their SNDMGR pointer argument (ApplyDialogEvent:
       lw 0x22C(soundManager + role * 4)), so they are members, not separate globals; and sprbuf.cpp's 64-byte aligned
       .bss (D_003DD2C0) starts where a 0x250-byte object ending at 0x3DD290 puts it, while the earlier 0x22C-byte
       definition left a 0x40-byte gap the link had to assign. No constructor or destructor touches them. */
    BASIC *dialogRoleObjects[6]; /* +0x22C */
    int dialogEventIndex;    /* +0x244 */
    float dialogEventTime;   /* +0x248 */
    float dialogElapsed;     /* +0x24C */
    // Construct the sound manager through SNDMGR_ctor.
    SNDMGRG() { SNDMGR_ctor(this); }
    // Empty destructor.
    ~SNDMGRG() {}
};
SNDMGRG D_003DD040;
/* The definition is the whole SNDMGR on every target; 0x250 bytes on the EE (sly2/sndmgr.h). */
typedef char assert_SNDMGRG_size[sizeof(SNDMGRG) == sizeof(SNDMGR) ? 1 : -1];
typedef char assert_SNDMGRG_tail[SLY2_OFFSETOF(SNDMGRG, dialogRoleObjects) == SLY2_OFFSETOF(SNDMGR, dialogRoleObjects) ? 1 : -1];

// 0x1FF188 StaticInit_sound (g_sndmgr, then the six slot pools; g_sndmgr's destruction) and its stubs.
SLY2_INSTANTIATE_SLOT_POOLS()

/* A sound's request / state flags (+0x50): requests for the next flush (UpdateSnd), dirty parameters, and state. */
#define SND_START 0x1
#define SND_PAUSE 0x2
#define SND_CONTINUE 0x4
#define SND_STOP 0x8
#define SND_DIRTY_VOLUME 0x10
#define SND_DIRTY_PAN 0x20
#define SND_DIRTY_PITCH 0x40
#define SND_DIRTY_GROUP_VOLUME 0x80   /* STREAMSND sub-groups */
#define SND_DIRTY_GROUP_PAN 0x100
#define SND_POSITIONAL 0x200          /* has a source object or position: UpdateSnd runs the positional slot */
#define SND_QUERY_PLAYING 0x400       /* ask whether it is still playing (looping sounds) */
#define SND_TIMED 0x800               /* STREAMSND: its play time advances (inferred) */
#define SND_QUERY_STARTED 0x1000      /* STREAMSND: ask whether the stream has started */
#define SND_ALL 0x1FFF
#define SND_DIRTY_SOUND_PARAMS (SND_DIRTY_VOLUME | SND_DIRTY_PAN | SND_DIRTY_PITCH)
#define SND_DIRTY_GROUP_PARAMS (SND_DIRTY_GROUP_VOLUME | SND_DIRTY_GROUP_PAN)
#define SND_DIRTY_PARAMS (SND_DIRTY_SOUND_PARAMS | SND_DIRTY_GROUP_PARAMS)
#define SND_QUERIES (SND_QUERY_PLAYING | SND_TIMED | SND_QUERY_STARTED)
/* The sound manager as the m2c bodies read it.
   SNDMGR layout (0x250 bytes; each function below has its own partial view, all with these names):
     +0x10  isHeadsetEnabled      the "use headset" option (save.cpp loads it from psvpUseHeadset; menu.cpp shows it)
     +0x14  unknown14             cleared by ResetSndmgr only
     +0x18  numSfx                size of the sfx id -> sound index table (ResetSndmgr: -1)
     +0x1C  bankSoundPool, +0x34 streamSoundPool (SLOTHEAPs), +0x4C dirtySounds (DL)
     +0x58  isPaused              set while the disc is busy (UpdateSndmgr pauses the music / ambience / dialog)
     +0x5C  musicLayer2Requests, +0x60 musicLayer0Requests (counted requests), +0x64 selectedMusicLayer (0..2)
     +0x68  muteWhenStoppedFlags  bit 1: cleared by a player mode, bit 2: cleared during cutscenes; when both are set
                                  (3, the default) the ambience and music fade out while game time is stopped
     +0x6C  isMicLevelQueryBusy, +0x70 micLevel (headset microphone level from the IOP, SUKR command 0xB),
     +0x74  micNoise              the NOISE the microphone makes for guards (ReplaceHeldNoise, noise.cpp)
     +0x80  micNoiseRangeCurve, +0x90 micNoiseHeightCurve (CLQ: microphone level -> noise range / height)
     +0xA0  isStatusQueryBusy, +0xA4 statusResult (SUKR command 0xC; meaning unknown)
     +0xA8  bankRecord (file record), +0xE8 bank (989snd bank handle)
     +0xEC  firstDialogStream     dialog lines (SNDSTREAM: per-language record, speaker, duration, lip-sync events)
     +0xF0  firstMusicFile        music tracks (SNDFILE: one language-independent record + id). Evidence: FindMusicStream
                                  walks it for PushMusic / FindMusic (which create kind-4 STREAMSNDs on the music stack),
                                  and GetStreamVolumeChannel maps its records to channel 1, which SetSndmgrChannelVolume
                                  applies to the music stack (+0x208); channel 2 (anything not music or ambience, i.e.
                                  the dialog streams) goes to the dialog sound (+0x200); and the options menu's music
                                  slider (menu.cpp) sets channel 1. The pass-2 comment calling it the dialog channel was
                                  wrong.
     +0xF4  ambienceRecord, +0xF8 numSounds, +0xFC soundKinds (per sound: 2 = looping), +0x100 soundIndexFromSfx
     +0x104 playbackMode (snd_SetPlaybackMode), +0x108 musicLayerWeights[3], +0x114 musicLayerSmoothing[3] (SMP)
     +0x138 sfxVariationStep      the random walk of StepRandomSfxVariation (-12..12)
     +0x13C volumes[8][4] (category x channel), +0x1BC masterVolume, +0x1C0 channelVolumes[4] (their product)
     +0x1D0 numAmbienceGroups, +0x1D4 ambienceGroupIds[6]
     +0x1F0 tickNow, +0x1F8 tickLast
     +0x200 dialogSound, +0x204 musicStackCount, +0x208 musicStack[8], +0x228 ambienceSound (counted pointers)
     +0x22C dialogRoleObjects[6]  per lip-sync role (3: the speaker object; the others its state machines)
     +0x244 dialogEventIndex, +0x248 dialogEventTime, +0x24C dialogElapsed
   Volume channels: 0 ambience, 1 music, 2 dialog, 3 bank sound effects (the menu's effects slider sets 0 and 3).
   Volume categories (rows of volumes): 1 the options menu, 2 the pause fade (0 pauses the channel's sounds), 3 a player
   mode's ducking (mode.cpp: 0.6); the rest are 1. */
#define G_SNDMGR (*(SNDMGR *)&D_003DD040)   /* the field layout of g_sndmgr (sly2/sndmgr.h) */
#define MUTE_WHEN_STOPPED_ALL 3   /* muteWhenStoppedFlags: both flags set (the default) */
#define PLAY_TIME_TICKS_PER_SECOND 294912000.0f   /* a stream's play time (+0x68) counts these */
#define CD_STATE_IDLE 3   /* cd.cpp's disc-poll state g_cds (1 read, 2 wait, 3 idle) */

extern "C" {
void SetSndFlags(CID151 *sound, int grfMask, int grf);
CID151 *SND_ctor(CID151 *sound);
BANKSND *BANKSND_ctor(BANKSND *sound, int sfxId);
void *AllocBanksnd(int cb);
int *FindSoundForSfxId(SNDMGR *soundManager, int sfxId);
void snd_PauseSound(int voiceHandle);
void snd_ContinueSound(int voiceHandle);
void *AllocZeroedFromSlotheap(SLOTHEAP *pslotheap);
void FreeSlotheapSlot(SLOTHEAP *pslotheap, void *pv);
void snd_DoExternCallNoWait(int nRpc, int nCmd, int n, int n3, int n4, int n5, int n6);   /* the IOP sound RPC */
SNDSTREAM *FindDialogStream(int id);
void snd_SetPlaybackMode(int nMode);
}
int IsEntryInDl(DL *pdl, void *pv);
void AppendDlEntry(DL *pdl, void *pv);

namespace M_NewSfx {
void *AllocClearedFromWorldHeap(SWALLOC *pmsg) __asm__("AllocClearedFromWorldHeap");   /* PvAllocSwClear */
struct E { int sfxId; float rangeNear; float rangeFar; float volume; int unknown10; };   /* SFX */
// Allocates a sound-effect descriptor (as objects load one from the level) on the world heap: no effect, full volume,
// audible from 300 (full) to 3000 units (silent).
extern "C" struct E *NewSfx(void)
{
    SWALLOC msg;
    struct E *sfx;
    msg.cb = sizeof(struct E);
    msg.cbAlign = 0x10;
    sfx = (__typeof__(sfx))AllocClearedFromWorldHeap(&msg);
    sfx->sfxId = -1;
    sfx->rangeNear = 300.0f;
    sfx->rangeFar = 3000.0f;
    sfx->volume = 1.0f;
    return sfx;
}
} // namespace M_NewSfx

namespace M_SND_ctor {
// Base sound constructor: BASIC header cleared, SND vtable, volume 1, audible from 300 to 3000 units.
extern "C" CID151 *SND_ctor(CID151 *sound)
{
    SLY2_SET_VPTR(sound, CID151);   /* the vptr, by hand (a C-named constructor) */
    sound->scriptFrame.m_p = 0;
    sound->scriptVariables.m_p = 0;
    sound->volume = 1.0f;
    sound->effectiveVolume = 1.0f;
    sound->rangeNear = 300.0f;
    sound->rangeFar = 3000.0f;
    return sound;
}
} // namespace M_SND_ctor

namespace M_PauseSnd {
// Pauses a sound (counted: nested pauses need as many resumes); the first pause requests "pause" on the next flush.
// Also known as: Pause (upstream readable / Sly 1 name).
extern "C" void PauseSnd(CID151 *sound) { if (++sound->pauseCount == 1) SetSndFlags(sound, SND_PAUSE | SND_CONTINUE, SND_PAUSE); }
} // namespace M_PauseSnd

namespace M_ResumeSnd {
// Undoes one PauseSnd; the last one requests "continue" on the next flush.
// Also known as: Resume (upstream readable / Sly 1 name).
extern "C" void ResumeSnd(CID151 *sound) { if (--sound->pauseCount == 0) SetSndFlags(sound, SND_PAUSE | SND_CONTINUE, SND_CONTINUE); }
} // namespace M_ResumeSnd

// Requests that a sound stop, fading out over dtFade seconds (applied by the next UpdateSnd).
// Also known as: Stop (upstream readable / Sly 1 name; Stop__4CSndf).
extern "C" void StopSnd(CID151 *sound, float fadeTime)
{
    sound->fadeTime = fadeTime;
    SetSndFlags(sound, SND_ALL, SND_STOP);
}

namespace M_StartSnd {
extern qword D_002A2DD0 __asm__("D_002A2DD0");
int AreFloatsNear__Ffff(float, float, float) __asm__("AreFloatsNear__Ffff");
// True when a and b are equal within 0.0001 on x, y and z.
extern "C" { static inline int AreVectorsNear(qword a, qword b)
{
    return AreFloatsNear__Ffff(QwordX(a), QwordX(b), 0.0001f) && AreFloatsNear__Ffff(QwordY(a), QwordY(b), 0.0001f) && AreFloatsNear__Ffff(QwordZ(a), QwordZ(b), 0.0001f);
} }
} // namespace M_StartSnd
// Starts a sound (slot 7) unless it is already playing or starting: requests start and marks every parameter dirty
// (plus pause if paused, plus positional if it has a source).
// Also known as: Start (upstream readable / Sly 1 name).
void CID151::Start()   /* key function: CID151's vtable comes out here (the namespace's views) */
{
    using namespace M_StartSnd;
    CID151 *sound = this;
    int isStopped = 0;
    int requests;
    if (!sound->voiceHandle) {
        isStopped = sound->playState || (sound->flags & SND_START);
        isStopped ^= 1;
    }
    if (!isStopped) return;
    { int isPaused = sound->pauseCount > 0; requests = SND_START | SND_DIRTY_PARAMS; if (isPaused) requests = SND_START | SND_PAUSE | SND_DIRTY_PARAMS; }
    if (sound->source || !AreVectorsNear(D_002A2DD0, sound->position))
        requests |= SND_POSITIONAL;
    SetSndFlags(sound, requests, requests);
}

namespace M_func_001FB1F8 {
struct S;
extern struct S g_lmZeroOne __asm__("g_lmZeroOne");
float ClampToLimits__FP2LMf(struct S *, float) __asm__("ClampToLimits__FP2LMf");
// Sets a sound's volume (clamped to 0..1); marks it dirty only if it changed by at least 1/1024.
// Also known as: SetVolume (upstream readable / Sly 1 name; SetVolume__4CSndf).
extern "C" void SetSndVolume(CID151 *sound, float volume)
{
    float clamped = ClampToLimits__FP2LMf(&g_lmZeroOne, volume);
    if ((int)(clamped * 1024.0f) != (int)(sound->volume * 1024.0f)) {
        sound->volume = clamped;
        SetSndFlags(sound, SND_DIRTY_VOLUME, SND_DIRTY_VOLUME);
    }
}
} // namespace M_func_001FB1F8

// Sets a sound's pan angle in degrees (normalized to 0..359) and marks it dirty.
// Also known as: SetPan (upstream readable / Sly 1 name; SetPan__4CSndi).
extern "C" void SetSndPan(CID151 *sound, int deg)
{
    int panAngle = deg % 360;
    panAngle = (panAngle > -1) ? panAngle : panAngle + 360;
    if (panAngle != sound->panAngle) {
        sound->panAngle = panAngle;
        SetSndFlags(sound, SND_DIRTY_PAN, SND_DIRTY_PAN);
    }
}

// Sets a sound's pitch and marks it dirty.
// Also known as: SetPitch (upstream readable / Sly 1 name; SetPitch__4CSndiif).
extern "C" void SetSndPitch(CID151 *sound, int n1, int n2, float pitchRatio)
{
    if (pitchRatio != sound->pitchRatio) {
        sound->pitchRatio = pitchRatio;
        SetSndFlags(sound, SND_DIRTY_PITCH, SND_DIRTY_PITCH);
    }
}

namespace M_SetSndSourceLo {
extern qword D_002A2DD0 __asm__("D_002A2DD0");
int AreFloatsNear__Ffff(float, float, float) __asm__("AreFloatsNear__Ffff");
void SetSndFlags(CID151 *, unsigned short, unsigned short) __asm__("SetSndFlags");
// True when a and b are equal within 0.0001 on x, y and z.
extern "C" { static inline int AreVectorsNear(qword a, qword b)
{
    return AreFloatsNear__Ffff(QwordX(a), QwordX(b), 0.0001f) && AreFloatsNear__Ffff(QwordY(a), QwordY(b), 0.0001f) && AreFloatsNear__Ffff(QwordZ(a), QwordZ(b), 0.0001f);
} }
// Makes a sound positional, following an object.
// Also known as: SetSource (upstream readable / Sly 1 name; SetSource__4CSndPv).
extern "C" void SetSndSourceLo(CID151 *sound, LO *source)
{
    sound->source = source;
    if (source) SetSndFlags(sound, SND_POSITIONAL, SND_POSITIONAL);
    else if (AreVectorsNear(D_002A2DD0, sound->position)) SetSndFlags(sound, SND_POSITIONAL, 0);
}
} // namespace M_SetSndSourceLo

namespace M_SetSndSourcePos {
extern qword D_002A2DD0 __asm__("D_002A2DD0");
int AreFloatsNear__Ffff(float, float, float) __asm__("AreFloatsNear__Ffff");
// True when a and b are equal within 0.0001 on x, y and z.
extern "C" { static inline int AreVectorsNear(qword a, qword b)
{
    return AreFloatsNear__Ffff(QwordX(a), QwordX(b), 0.0001f) && AreFloatsNear__Ffff(QwordY(a), QwordY(b), 0.0001f) && AreFloatsNear__Ffff(QwordZ(a), QwordZ(b), 0.0001f);
} }
// Makes a sound positional at a fixed world position (a zero vector means not positional).
// Also known as: SetPosition (upstream readable / Sly 1 name; SetPosition__4CSndG9VU_VECTOR).
extern "C" void SetSndSourcePos(CID151 *sound, qword position)
{
    sound->position = position;
    if (!AreVectorsNear(D_002A2DD0, position)) SetSndFlags(sound, SND_POSITIONAL, SND_POSITIONAL);
    else if (!sound->source) SetSndFlags(sound, SND_POSITIONAL, 0);
}
} // namespace M_SetSndSourcePos

namespace M_SetSndRange {
// Sets the distance at which a positional sound starts to fade (full) and the distance where it is silent (stop);
// stop is forced to be at least full + 1.
// Also known as: SetFalloff (upstream readable / Sly 1 name; SetFalloff__4CSndff).
extern "C" void SetSndRange(CID151 *sound, float rangeNear, float rangeFar) { if (rangeFar <= rangeNear) { rangeFar = rangeNear + 1.0f; sound->rangeFar = rangeFar; } else sound->rangeFar = rangeFar; sound->rangeNear = rangeNear; }
} // namespace M_SetSndRange

/* SetSndDoppler: empty */
// Empty in the release build; called with a fade/doppler argument (inferred).
extern "C" void SetSndDoppler()
{
}

namespace M_UpdateSndPositional {
typedef unsigned long ulong;
qword ConvertPosBetweenLos(LO *, int, qword) __asm__("ConvertPosBetweenLos");
void CalculateSndVolPan(CID151 *, qword, float *, float *, int *) __asm__("CalculateSndVolPan");
// Positional update (slot 16): volume from the distance to the listener and pan from its direction; switches the
// sound's culled state when it becomes audible or silent.
// Also known as: Update3D (upstream readable / Sly 1 name).
extern "C" void UpdateSndPositional(CID151 *sound)
{
    float fade;
    int panAngle;
    float volume;
    if (sound->source && ((int)sound->source->isInWorld & 1) == 0) {
        StopSnd(sound, 0.0f);
        return;
    }
    CalculateSndVolPan(sound, ConvertPosBetweenLos(sound->source, 0, sound->position), &sound->rangeNear, &fade, &panAngle);
    volume = sound->volume * fade;
    if ((int)(volume * 1024.0f) != (int)(sound->effectiveVolume * 1024.0f)) {
        if (sound->effectiveVolume == 0.0f) sound->SetCulled(0);
        else if (volume == 0.0f) sound->SetCulled(1);
        sound->effectiveVolume = volume;
        SetSndFlags(sound, SND_DIRTY_VOLUME, SND_DIRTY_VOLUME);
    }
    SetSndPan(sound, panAngle);
}
} // namespace M_UpdateSndPositional

namespace M_SetSndFlags {
void AddSndToDirtyList(CID151 *) __asm__("AddSndToDirtyList");
void RemoveSndFromDirtyList(CID151 *) __asm__("RemoveSndFromDirtyList");
// Sets the masked bits of a sound's request flags; a sound with pending requests is put on the manager's dirty list,
// one without is taken off it.
// Also known as: SetDirty (upstream readable / Sly 1 name; SetDirty__4CSndUiUi).
void SetSndFlags(CID151 *sound, unsigned short mask, unsigned short value) __asm__("SetSndFlags"); void SetSndFlags(CID151 *sound, unsigned short mask, unsigned short value)
{
    sound->flags = (sound->flags & ~mask) | (value & mask);
    if (sound->flags) AddSndToDirtyList(sound); else RemoveSndFromDirtyList(sound);
}
} // namespace M_SetSndFlags

// Puts a sound on the manager's dirty list (taking a reference) if it is not already there.
extern "C" void AddSndToDirtyList(CID151 *sound)
{
    if (IsEntryInDl(&G_SNDMGR.dirtySounds, sound) == 0) {
        sound->AddRef();
        AppendDlEntry(&G_SNDMGR.dirtySounds, sound);
    }
}

namespace M_RemoveSndFromDirtyList {
struct DL;
int IsEntryInDl__FP2DLPv(struct DL *, void *) __asm__("IsEntryInDl__FP2DLPv");
void RemoveDlEntry__FP2DLPv(struct DL *, void *) __asm__("RemoveDlEntry__FP2DLPv");
// Takes a sound off the dirty list and drops the list's reference.
// Also known as: RemoveActive (upstream readable / Sly 1 name).
extern "C" void RemoveSndFromDirtyList(CID151 *sound)
{
    if (IsEntryInDl__FP2DLPv((struct DL *)&D_003DD040.dlSndDirty, sound)) {
        RemoveDlEntry__FP2DLPv((struct DL *)&D_003DD040.dlSndDirty, sound);
        sound->Release();
    }
}
} // namespace M_RemoveSndFromDirtyList

namespace M_CalculateSndVolPan {
struct RANGE {float low,high;};
extern char g_lmZeroOne[] __asm__("g_lmZeroOne");
void *GetCurrentPo(void*) __asm__("GetCurrentPo");
qword GetLoWorldPos(void*) __asm__("GetLoWorldPos");
qword ConvertMatVec(const struct MATRIX3Q*,const struct MATRIX3Q*,qword) __asm__("ConvertMatVec");
float ClampToLimits__FP2LMf(void*,float) __asm__("ClampToLimits__FP2LMf");
float atan2f(float,float) __asm__("atan2f");
// Return a - b (vf3).
extern "C" { static inline qword Sub(qword a,qword b){register qword r __asm__("vf3");__asm__("vsub.xyzw %0,%1,%2":"=&j"(r):"j"(a),"j"(b));return r;} }
// Length of the xyz part of a as a float; also stores a to *save.
extern "C" { static inline float Length(qword a,QWROW*save){qword d,one;float g;
__asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=&j"(d),"=&j"(one):"j"(a));
__asm__("vsqrt $Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q":"=j"(d):"j"(d));
save->q=a;
__asm__("qmfc2.ni %0,%1":"=r"(g):"j"(d));return g;} }
// y lane of q as a float.
extern "C" { static inline float Y(qword q){int bits;float f;__asm__("prot3w %0,%1":"=r"(bits):"r"(q));__asm__("mtc1 %1,%0":"=f"(f):"r"(bits));return f;} }
// Move a VU0 register to a 128-bit GPR value.
extern "C" { static inline qword Bits(qword v){qword q;__asm__("qmfc2.ni %0,%1":"=r"(q):"j"(v));return q;} }
// Round f to int (cvt.w.s) and store it to *out.
extern "C" { static inline void Rounded(int*out,float f){float bits;__asm__("cvt.w.s %0,%1":"=&f"(bits):"f"(f));__asm__("swc1 %1,%0":"=m"(*out):"f"(bits));} }
// Volume and pan of a point as heard by the listener (the player, or the camera): volume falls linearly from 1 at the
// full distance to 0 at the stop distance; pan is the angle to the point in degrees.
extern "C" void CalculateSndVolPan(void *unused,qword point,const struct RANGE*limits,float*outValue,int*outAngle)
{
 QWROW source,delta;
 void *player;
 register qword reference __asm__("vf1");
 float high,low,distance,value;
 register qword difference __asm__("vf3");
 source.q=point;
 player=GetCurrentPo(&g_sw);
 if(player)reference=GetLoWorldPos(player);
 else reference=g_cm.matq.pos;
 high=limits->high;low=limits->low;
 {register qword input __asm__("vf2")=source.q;
  distance=Length(Sub(input,reference),&delta);}
 value=ClampToLimits__FP2LMf(g_lmZeroOne,(high-distance)/(high-low));
 difference=delta.q;
 if(outValue)*outValue=value;
 if(outAngle){
   if(value>0.0f){
     qword direction;
     float angle;
     direction=ConvertMatVec(0,&(*(struct MATRIX3Q *)&g_cm.mat),Bits(difference));
     angle=atan2f(-Y(direction),QwordX(direction));
     if(limits->high<limits->low)angle+=3.1415927f;
     Rounded(outAngle,angle*57.295776f);return;
   }else *outAngle=0;
 }
}
} // namespace M_CalculateSndVolPan

namespace M_UpdateSnd {
/* holds a reference for the duration of the update */
struct Lock {
    CID151 *sound;
    // Scope guard: add a reference to the sound on entry and drop it on exit.
    Lock(CID151 *locked) { sound = locked; sound->AddRef(); }
    // Drop the reference taken on entry.
    ~Lock() { sound->Release(); }
};
// Flushes a dirty sound to the sound hardware: positional update, then start, stop, status query, parameters, pause and
// continue by its request bits (each through its virtual member).
// Also known as: UpdateFrame (upstream readable / Sly 1 name).
extern "C" void UpdateSnd(CID151 *sound)
{
    Lock lock(sound);
    unsigned short flags;
    if (sound->flags & SND_POSITIONAL) sound->UpdatePositional();
    else sound->effectiveVolume = sound->volume;
    flags = sound->flags;
    if (flags & SND_START) {
        sound->Play();
        if (sound->GetKind() == 1) SetSndFlags(sound, SND_QUERY_PLAYING, SND_QUERY_PLAYING);   /* one-shot: watch for its end */
    } else if (sound->voiceHandle) {
        if (flags & SND_STOP) {
            sound->Stop();
            sound->voiceHandle = 0;
            SetSndFlags(sound, SND_ALL & ~SND_POSITIONAL, 0);
        } else {
            if (flags & SND_QUERIES) sound->QueryStatus();
            if (sound->playState) return;   /* still starting */
            if (sound->flags & SND_DIRTY_PARAMS) {
                sound->ApplyParams();
                SetSndFlags(sound, SND_DIRTY_PARAMS, 0);
            }
            flags = sound->flags;
            if (flags & SND_PAUSE) {
                sound->Pause();
                SetSndFlags(sound, SND_PAUSE, 0);
            } else if (flags & SND_CONTINUE) {
                sound->Resume();
                SetSndFlags(sound, SND_CONTINUE, 0);
            }
        }
    } else if (!sound->playState && !(flags & SND_POSITIONAL)) {
        SetSndFlags(sound, SND_ALL, 0);   /* not playing and nothing to follow: off the dirty list */
    }
}
} // namespace M_UpdateSnd
#undef VCALL
#undef ICALL

namespace M_func_001FBAF8 {
// Async callback when a looping bank sound started: stores its voice handle, clears "starting", drops the callback's
// reference. (inferred)
extern "C" void OnBankSoundStarted(int voiceHandle, long context) { CID151 *sound = (CID151 *)context; sound->voiceHandle = voiceHandle; sound->playState = 0; sound->Release(); }
} // namespace M_func_001FBAF8

namespace M_func_001FBB30 {
// Async "is it still playing" callback: a finished sound is stopped, a playing one gets its next status query armed;
// drops the callback's reference. (inferred)
extern "C" void OnBankSoundStatus(int isPlaying, long context)
{
    CID151 *sound = (CID151 *)context;
    if (!isPlaying)
        StopSnd(sound, 0.0f);
    else
        SetSndFlags(sound, SND_QUERY_PLAYING, SND_QUERY_PLAYING);
    sound->Release();
}
} // namespace M_func_001FBB30

// Creates a bank sound for a sound-effect id from the manager's pool; it starts at the next flush.
// Also known as: PsndNewPlain (upstream readable / Sly 1 name; PsndNewPlain__Fi).
extern "C" void CreateSfxSound(int sfxId)
{
    BANKSND_ctor((BANKSND *)AllocBanksnd(0x70), sfxId);
}

// Bank-sound constructor: base sound constructor, then the start request.
extern "C" BANKSND *BANKSND_ctor(BANKSND *sound, int sfxId)
{
    SND_ctor(sound);
    sound->sfxId = sfxId;
    SLY2_SET_VPTR(sound, BANKSND);   /* the vptr, stored by hand (a C-named constructor) */
    SetSndFlags(sound, SND_START, SND_START);
    return sound;
}

// Kind of a bank sound (slot 8): 2 for looping effects, from the level's sound table, else -1.
extern "C" int GetBanksndKind(BANKSND *sound)
{
    int *pn = FindSoundForSfxId(&G_SNDMGR, sound->sfxId);
    int n = -1;
    if (pn != 0) {
        n = *pn;
    }
    return n;
}

namespace M_PlayBanksnd {
/* 989snd play parameters (0x30 bytes): the fields named in mask are used */
struct M { int bank; int pad4; int volume; int pan; short soundIndex; short pad12; int pad14[3]; short pitch; short pad22; int pad24; int mask; int pad2C; };
#define PLAY_PARAM_BANK 1
#define PLAY_PARAM_SOUND 2
#define PLAY_PARAM_VOLUME 4
#define PLAY_PARAM_PAN 8
#define PLAY_PARAM_PITCH 0x10
#define SOUND_KIND_LOOPING 2
#define SFX_SLY 0x6F       /* the three heroes' effects that drive StepRandomSfxVariation (inferred) */
#define SFX_BENTLEY 0x73
#define SFX_MURRAY 0x15
extern SNDMGR D_003DD040 __asm__("D_003DD040");
extern LO *D_002DD5BC __asm__("D_002DD5BC"), *D_002F7900 __asm__("D_002F7900"), *D_002E1E40 __asm__("D_002E1E40");   /* Bentley, Murray, Sly */
short GetSoundIndex(SNDMGR *, int *) __asm__("GetSoundIndex");
void RemoveSndFromDirtyList(CID151 *) __asm__("RemoveSndFromDirtyList");
void *func_002333B4(void *, int, int) __asm__("memset");
void OnBankSoundStarted(void) __asm__("OnBankSoundStarted");
void snd_PlaySoundEx_CB(struct M *, void (*)(void), BANKSND *) __asm__("snd_PlaySoundEx_CB");
void snd_PlaySoundEx(struct M *) __asm__("snd_PlaySoundEx");
void *GetHeroPo(void *) __asm__("GetHeroPo");
void StepRandomSfxVariation(SNDMGR *) __asm__("StepRandomSfxVariation");
// Starts a bank sound on the 989snd library (slot 9) with its bank, sound index, volume, pan and pitch: loopers start
// asynchronously and get a voice handle, one-shots are fire-and-forget; some effects get special handling.
// Also known as: StartIop (upstream readable / Sly 1 name).
extern "C" void PlayBanksnd(BANKSND *sound)
{
    SNDMGR *soundManager = &D_003DD040;
    int *entry = FindSoundForSfxId(soundManager, sound->sfxId);
    struct M params;
    LO *owner;
    if (!entry || !soundManager->bank) {
        SetSndFlags(sound, SND_ALL, 0);
        return;
    }
    if (sound->flags & SND_PAUSE) {
        RemoveSndFromDirtyList(sound);
        return;
    }
    func_002333B4(&params, 0, sizeof(params));
    params.mask = PLAY_PARAM_BANK | PLAY_PARAM_SOUND;
    params.bank = soundManager->bank;
    params.soundIndex = GetSoundIndex(soundManager, entry);
    if (sound->flags & SND_DIRTY_VOLUME) {
        params.mask |= PLAY_PARAM_VOLUME;
        params.volume = (int)(sound->effectiveVolume * 1024.0f);
    }
    if (sound->flags & SND_DIRTY_PAN) {
        params.mask |= PLAY_PARAM_PAN;
        params.pan = sound->panAngle;
    }
    if (sound->flags & SND_DIRTY_PITCH) {
        params.mask |= PLAY_PARAM_PITCH;
        params.pitch = (int)(sound->pitchRatio * 1524.0f);
    }
    if (*entry == SOUND_KIND_LOOPING) {
        sound->playState = 1;
        sound->AddRef();
        snd_PlaySoundEx_CB(&params, OnBankSoundStarted, sound);
        SetSndFlags(sound, SND_START | SND_PAUSE | SND_CONTINUE | SND_STOP | SND_DIRTY_SOUND_PARAMS, 0);
        return;
    }
    owner = 0;
    snd_PlaySoundEx(&params);
    SetSndFlags(sound, SND_ALL, 0);
    switch (sound->sfxId) {
    case SFX_SLY: owner = D_002E1E40; break;
    case SFX_BENTLEY: owner = D_002DD5BC; break;
    case SFX_MURRAY: owner = D_002F7900; break;
    }
    if (owner == GetHeroPo(&g_sw)) StepRandomSfxVariation(&D_003DD040);
}
} // namespace M_PlayBanksnd

namespace M_StopBanksnd {
void snd_AutoVol(int voiceHandle, int a, int b, int c) __asm__("snd_AutoVol");
void snd_StopSound(int voiceHandle) __asm__("snd_StopSound");
// Stops a bank sound (slot 10): a fade-out when a fade time was requested, else immediately.
// Also known as: StopIop (upstream readable / Sly 1 name).
extern "C" void StopBanksnd(CID151 *sound)
{
    if (sound->fadeTime > 0.0f)
        snd_AutoVol(sound->voiceHandle, -4,   /* fade to silence over fadeTime (240 ticks per second) */
                     (int)(sound->fadeTime * 240.0f), 2);
    else
        snd_StopSound(sound->voiceHandle);
}
} // namespace M_StopBanksnd

// Pauses a bank sound's voice (slot 11).
// Also known as: PauseIop (upstream readable / Sly 1 name).
extern "C" void PauseBanksnd(CID151 *sound)
{
    snd_PauseSound(sound->voiceHandle);
}

// Resumes a bank sound's voice (slot 12).
// Also known as: ResumeIop (upstream readable / Sly 1 name).
extern "C" void ContinueBanksnd(CID151 *sound)
{
    snd_ContinueSound(sound->voiceHandle);
}

namespace M_ApplyBanksndParams {
void snd_SetSoundParams(int, int, int, int, int, int) __asm__("snd_SetSoundParams");   /* 989snd: set voice volume / pan / pitch (inferred) */
// Sends a bank sound's changed volume, pan and pitch to its voice (slot 13).
// Also known as: SendChanges (upstream readable / Sly 1 name).
extern "C" void ApplyBanksndParams(CID151 *sound)
{
    int flags = sound->flags;
    if (flags & SND_DIRTY_SOUND_PARAMS) {
        int changed = 0;
        int volume, pitch;
        if (flags & SND_DIRTY_VOLUME) { changed = 1; volume = (int)(sound->effectiveVolume * 1024.0f); }
        if (flags & SND_DIRTY_PAN) changed |= 4;
        if (flags & SND_DIRTY_PITCH) { changed |= 8; pitch = (int)(sound->pitchRatio * 12.0f); }
        snd_SetSoundParams(sound->voiceHandle, changed, volume, sound->panAngle, pitch, 0);
    }
}
} // namespace M_ApplyBanksndParams

namespace M_BANKSND_QueryStatus {
void OnBankSoundStatus(void) __asm__("OnBankSoundStatus");
void snd_SoundIsStillPlaying_CB(int, void (*)(void), void *) __asm__("snd_SoundIsStillPlaying_CB");
// Asks the IOP whether a bank sound's voice is still playing (slot 14), with OnBankSoundStatus as callback.
// Also known as: Requests (upstream readable / Sly 1 name).
extern "C" void BANKSND_QueryStatus(CID151 *sound)
{
    if (sound->flags & SND_QUERY_PLAYING) {
        sound->AddRef();
        snd_SoundIsStillPlaying_CB(sound->voiceHandle, OnBankSoundStatus, sound);
        SetSndFlags(sound, SND_QUERY_PLAYING, 0);
    }
}
} // namespace M_BANKSND_QueryStatus

namespace M_func_001FC018 {
// Culling hook (slot 15): a sound that becomes audible again restarts; one that becomes silent is stopped (loopers keep
// their parameters so they can restart). (inferred)
// Also known as: OnAudible (upstream readable / Sly 1 name; OnAudible__9CSndPlaini).
extern "C" void SetBankSoundCulled(CID151 *sound, int isCulled)
{
    if (!isCulled) sound->Start();
    else {
        /* request the stop, clearing the other requests; a looper keeps its positional flag so the positional update
           can restart it when it becomes audible again */
        int looperMask = SND_ALL & ~SND_POSITIONAL & ~SND_QUERIES;
        SetSndFlags(sound, sound->GetKind() == SOUND_KIND_LOOPING ? looperMask : SND_ALL & ~SND_QUERIES, SND_STOP);
    }
}
} // namespace M_func_001FC018

// Adds a reference to a bank sound (slot 1).
void BANKSND::AddRef()   /* key function: BANKSND's vtable comes out here */
{
    refCount++;
}

namespace M_DecrefBanksnd {
void BASIC_dtor(BANKSND *, int) __asm__("_$_5BASIC");   /* ~BASIC with in-charge flag 0 */
void FreeBanksnd(BANKSND *) __asm__("FreeBanksnd");
// Drops a reference to a bank sound (slot 2); at zero it is destroyed and returned to its pool. (inferred)
// Also known as: Release (upstream readable / Sly 1 name).
extern "C" void DecrefBanksnd(BANKSND *sound) { if (--sound->refCount == 0 && sound) { BASIC_dtor(sound, 0); FreeBanksnd(sound); } }
} // namespace M_DecrefBanksnd

// Allocates a bank sound from the manager's pool.
extern "C" void *AllocBanksnd(int cb)
{
    return AllocZeroedFromSlotheap(&G_SNDMGR.bankSoundPool);
}

// Returns a bank sound to the manager's pool.
extern "C" void FreeBanksnd(void *pv)
{
    FreeSlotheapSlot(&G_SNDMGR.bankSoundPool, pv);
}

/* The IOP stream module's RPC id ('SUKR') and the commands the stream sounds send (macros: an enum argument would
   make g++ look the per-function declarations up in the global namespace too, an ambiguous overload). */
#define SUKR_RPC 0x524B5553
#define SUKR_INIT 0
#define SUKR_START 1
#define SUKR_STOP 2
#define SUKR_PAUSE 3
#define SUKR_CONTINUE 4
#define SUKR_SET_PARAMS 5          /* volume / pan / pitch */
#define SUKR_SET_GROUP_PARAMS 6    /* a sub-group's volume / pan */
#define SUKR_QUERY_PLAYING 7       /* (inferred) */
#define SUKR_QUERY_POSITION 8      /* (inferred) */
#define SUKR_STOP_ALL 10           /* ResetSndmgr */
#define SUKR_QUERY_MIC_LEVEL 0xB   /* headset microphone level (UpdateHeadsetMicrophone; inferred) */
#define SUKR_QUERY_STATUS 0xC      /* periodic status (PollSoundManagerStatus; meaning unknown) */

namespace M_PstreamsndNew {
// Creates a streamed sound (music, dialog, ambience) for a stream record and kind, or 0 if the pool is empty.
// Also known as: PsndNewVoice (upstream readable / Sly 1 name; PsndNewVoice__FP9SNDSTREAMPv).
extern "C" STREAMSND *PstreamsndNew(FILEREC *pfilerec, int streamKind)
{
    STREAMSND *px;
    if (pfilerec)
        px = new STREAMSND(pfilerec, streamKind);
    else
        px = 0;
    return px;
}
} // namespace M_PstreamsndNew

namespace M_STREAMSND_ctor {
/* STREAMSND (0x3C0): SND, then the stream record (+0x60), play time (+0x68), six sub-groups' volumes (+0x70), pans
   (+0x88), source counts (+0xA0) and sources (+0xB8, 8 each), the stream kind (+0x3B8) and the dirty sub-group mask
   (+0x3BC). */
extern SNDMGR D_003DD040 __asm__("D_003DD040");
int GetStreamVolumeChannel(SNDMGR *, FILEREC *) __asm__("GetStreamVolumeChannel");
void PauseSnd(CID151 *) __asm__("PauseSnd");
// Streamed-sound constructor: base constructor, STREAMSND vtable, the file record and kind, all six sub-group volumes at
// 1; requests start and marks it timed, and starts paused when its channel's pause-fade weight is 0 or
// the manager is paused.
// Also known as: CSndVoice constructor (upstream readable / Sly 1 name; __9CSndVoiceii).
extern "C" STREAMSND *STREAMSND_ctor(STREAMSND *sound, FILEREC *pfilerec, int streamKind)
{
    int i;
    SNDMGR *soundManager;
    SND_ctor(sound);
    sound->record.pfilerec = pfilerec;
    SLY2_SET_VPTR(sound, STREAMSND);   /* the vptr, by hand (a C-named constructor) */
    sound->streamKind = streamKind;
    for (i = 0; i < 6; i++)
        sound->groupVolumes[i] = 1.0f;
    soundManager = &D_003DD040;
    SetSndFlags(sound, SND_START | SND_TIMED, SND_START | SND_TIMED);
    /* start paused when its channel is silent or the manager is paused */
    if (*(soundManager->volumes[VOLUME_CATEGORY_PAUSE_FADE] + GetStreamVolumeChannel(soundManager, sound->record.pfilerec)) == 0.0f)
        PauseSnd(sound);
    if (soundManager->isPaused)
        PauseSnd(sound);
    return sound;
}
} // namespace M_STREAMSND_ctor

namespace M_StartStreamsnd {
void StartSnd(STREAMSND *) __asm__("StartSnd");
// Starts a streamed sound (slot 7): base start, then resets its play time and status queries.
// Also known as: Start (upstream readable / Sly 1 name).
extern "C" void StartStreamsnd(STREAMSND *sound)
{
    /* only when not already started: no voice, not playing, flag 1 clear */
    int canStart = 0; if (!sound->voiceHandle) { canStart = sound->playState || (sound->flags & SND_START); canStart ^= 1; } if (canStart) {
        StartSnd(sound);
        sound->playTime = 0;
        sound->dirtyGroups = 0xF;
    }
}
} // namespace M_StartStreamsnd

namespace M_SetStreamsndGroupVolume {
struct S;
extern struct S g_lmZeroOne __asm__("g_lmZeroOne");   /* the volume limits (0..1) */
float ClampToLimits__FP2LMf(struct S *, float) __asm__("ClampToLimits__FP2LMf");
void SetSndFlags(STREAMSND *, int, int) __asm__("SetSndFlags");
// Sets the volume of one of a stream's six sub-groups (channels of a multi-layer stream) and marks it dirty.
// Also known as: SetVoiceVolume (upstream readable / Sly 1 name; SetVoiceVolume__9CSndVoiceif).
extern "C" void SetStreamsndGroupVolume(STREAMSND *sound, unsigned int group, float volume)
{
    if (group < 6) {
        float clamped = ClampToLimits__FP2LMf(&g_lmZeroOne, volume);
        /* compared at the IOP's 1/1024 resolution */
        if ((int)(clamped * 1024.0f) != (int)(sound->groupVolumes[group] * 1024.0f)) {
            sound->dirtyGroups |= 1 << group;
            sound->groupVolumes[group] = clamped;
            SetSndFlags(sound, SND_DIRTY_GROUP_VOLUME, SND_DIRTY_GROUP_VOLUME);
        }
    }
}
} // namespace M_SetStreamsndGroupVolume

namespace M_func_001FC380 {
void SetSndFlags(STREAMSND *, int, int) __asm__("SetSndFlags");
// Wrap an angle in degrees to 0..359.
extern "C" { static inline int Norm(int deg) { int n = deg % 360; return n > -1 ? n : n + 360; } }   /* to 0..359 */
// Sets the pan of one of a stream's six sub-groups and marks it dirty. (inferred)
// Also known as: SetVoicePan (upstream readable / Sly 1 name; SetVoicePan__9CSndVoiceii).
extern "C" void SetStreamGroupPan(STREAMSND *sound, unsigned int group, int deg)
{
    if (group < 6) {
        int pan = Norm(deg);
        if (pan != sound->groupPans[group]) {
            sound->dirtyGroups |= 1 << group;
            sound->groupPans[group] = pan;
            SetSndFlags(sound, SND_DIRTY_GROUP_PAN, SND_DIRTY_GROUP_PAN);
        }
    }
}
} // namespace M_func_001FC380

namespace M_func_001FC408 {
// Adds a positional source (object, volume, range) to a stream sub-group (at most 8 per group); the loudest audible
// source drives the group's volume and pan. (inferred)
// Also known as: AddVoiceFalloff (upstream readable / Sly 1 name; AddVoiceFalloff__9CSndVoiceiiPvf).
extern "C" void AddStreamGroupSource(STREAMSND *sound, unsigned int group, LO *object, LM *range, float gain)
{
    if (group < 6 && sound->groupSourceCounts[group] < 8) {
        SNDSOURCE *source;
        SetSndFlags(sound, SND_POSITIONAL, SND_POSITIONAL);
        source = &sound->groupSources[group][sound->groupSourceCounts[group]++];
        source->plo = object;
        source->gain = gain;
        source->range = *range;
        if (source->range.minValue == source->range.maxValue)
            source->range.maxValue += 1.0f;   /* keep the falloff range non-empty */
    }
}
} // namespace M_func_001FC408

namespace M_func_001FC4F8 {
// Kind of a streamed sound (slot 8): 2 when looping, else 1. (inferred)
// Also known as: Kind (upstream readable / Sly 1 name).
extern "C" int GetStreamSoundKind(STREAMSND *sound) { return (sound->streamKind & 4) ? 2 : 1; }
} // namespace M_func_001FC4F8

namespace M_PlayStreamsnd {
struct K { int keyLo; int keyHi; int kind; char filename; char pad[0x110 - 0xD]; };   /* a file lookup key (as hud.cpp's) */
/* the 'SUKR' start command's 0x18-byte payload */
struct M { int fileKey; int volume; int pan; int channel; int kind; int playOnHeadset; };
extern SNDMGR D_003DD040 __asm__("D_003DD040");
void InitFileKey(struct K *) __asm__("InitFileKey");
int GetStreamVolumeChannel(SNDMGR *, FILEREC *) __asm__("GetStreamVolumeChannel");
void PauseSnd(CID151 *) __asm__("PauseSnd");
void OnStreamSoundStarted(void) __asm__("OnStreamSoundStarted");
void snd_DoExternCallWithData_CB(int, int, int, struct M *, void (*)(void), STREAMSND *) __asm__("snd_DoExternCallWithData_CB");
// Build the stream file's decryption key from its record (kind 2, lo = (k0^k2)+0xCA0, hi = k1^k3); returns whether it
// is usable (hi != 0).
extern "C" { static inline int BuildStreamFileKey(struct K *key, FILEREC *record)
{
    if (!record) return 0;
    {
        int k2 = record->aw[13];   /* the key words at +0x34, +0x0, +0x3C, +0x4 */
        int k0 = record->aw[0];
        int k3 = record->aw[15];
        int k1 = record->aw[1];
        key->kind = 2;
        key->keyHi = k1 ^ k3;
        key->keyLo = (k0 ^ k2) + 0xCA0;
    }
    return key->keyHi != 0;
} }
// Starts a streamed sound (slot 9): resolves the stream's file and sends the start command to the IOP stream module
// with volume and pan.
// Also known as: StartIop (upstream readable / Sly 1 name).
extern "C" void PlayStreamsnd(STREAMSND *sound)
{
    struct K key;
    struct M message;
    int channel;
    key.kind = 2;
    InitFileKey(&key);
    channel = GetStreamVolumeChannel(&D_003DD040, sound->record.pfilerec);
    if (BuildStreamFileKey(&key, sound->record.pfilerec)) {
        if (key.kind != 2) {
            SetSndFlags(sound, SND_ALL, 0);
            return;
        }
        message.fileKey = key.keyLo;
        PauseSnd(sound);
        message.volume = (int)(sound->effectiveVolume * 1024.0f);
        message.pan = sound->panAngle;
        message.channel = channel;
        message.kind = sound->streamKind | 1;
        {
            /* with the headset option on, dialog lines play through the headset unless their record says otherwise (inferred) */
            int playOnHeadset = 0;
            if (D_003DD040.isHeadsetEnabled && channel == VOLUME_CHANNEL_DIALOG)
                playOnHeadset = sound->record.psndstream->excludeFromHeadset == 0;
            message.playOnHeadset = playOnHeadset;
        }
        sound->playState = 1;
        sound->AddRef();   /* for the callback */
        snd_DoExternCallWithData_CB(SUKR_RPC, SUKR_START, 0x18, &message, OnStreamSoundStarted, sound);
        SetSndFlags(sound, SND_START | SND_PAUSE | SND_CONTINUE | SND_STOP | SND_DIRTY_VOLUME | SND_DIRTY_PAN, 0);
    } else {
        SetSndFlags(sound, SND_ALL, 0);
    }
}
} // namespace M_PlayStreamsnd

// Stops a streamed sound on the IOP (slot 10). (inferred)
// Also known as: StopIop (upstream readable / Sly 1 name).
extern "C" void StopStreamSound(CID151 *sound)
{
    snd_DoExternCallNoWait(SUKR_RPC, SUKR_STOP, sound->voiceHandle, 0, 0, 0, 0);
}

// Pauses a streamed sound on the IOP (slot 11). (inferred)
// Also known as: PauseIop (upstream readable / Sly 1 name).
extern "C" void PauseStreamSound(CID151 *sound)
{
    snd_DoExternCallNoWait(SUKR_RPC, SUKR_PAUSE, sound->voiceHandle, 0, 0, 0, 0);
}

// Resumes a streamed sound on the IOP (slot 12). (inferred)
// Also known as: ResumeIop (upstream readable / Sly 1 name).
extern "C" void ResumeStreamSound(CID151 *sound)
{
    snd_DoExternCallNoWait(SUKR_RPC, SUKR_CONTINUE, sound->voiceHandle, 0, 0, 0, 0);
}

namespace M_ApplyStreamsndParams {
// Sends a stream's changed volume, pan and pitch, then each sub-group's parameters (slot 13).
// Also known as: SendChanges (upstream readable / Sly 1 name).
extern "C" void ApplyStreamsndParams(STREAMSND *sound)
{
    int i;
    /* sound flags: 0x10 volume, 0x20 pan, 0x40 pitch dirty; 0x80 / 0x100 sub-group volume / pan dirty */
    if (sound->flags & SND_DIRTY_SOUND_PARAMS) {
        int changed = 0;
        int volume, pitch;
        if (sound->flags & SND_DIRTY_VOLUME) {
            changed = 1;
            volume = (int)(sound->effectiveVolume * 1024.0f);
        }
        if (sound->flags & SND_DIRTY_PAN)
            changed |= 4;
        if (sound->flags & SND_DIRTY_PITCH) {
            changed |= 8;
            pitch = (int)(sound->pitchRatio * 12.0f);
        }
        snd_DoExternCallNoWait(SUKR_RPC, SUKR_SET_PARAMS, sound->voiceHandle, changed, volume, sound->panAngle, pitch);
    }
    if (sound->flags & SND_DIRTY_GROUP_PARAMS) {
        for (i = 0; i < 6; i++) {
            int mask = 1 << i;
            if (sound->dirtyGroups & mask)
                snd_DoExternCallNoWait(SUKR_RPC, SUKR_SET_GROUP_PARAMS, sound->voiceHandle, i, (int)(sound->groupVolumes[i] * 1024.0f), sound->groupPans[i], 0);
        }
        sound->dirtyGroups = 0;
    }
}
} // namespace M_ApplyStreamsndParams

namespace M_func_001FC878 {
extern SNDMGR D_003DD040 __asm__("D_003DD040");
void OnStreamsndStatus(void) __asm__("OnStreamsndStatus");
void OnBankSoundStatus(void) __asm__("OnBankSoundStatus");
void snd_DoExternCall_CB(int, int, int, int, int, int, int, void (*)(void), long) __asm__("snd_DoExternCall_CB");
// Queries a stream's play position and whether it is still playing (slot 14), with callbacks; accumulates paused time.
// (inferred)
// Also known as: Requests (upstream readable / Sly 1 name).
extern "C" void QueryStreamSoundStatus(STREAMSND *sound)
{
    if (sound->flags & SND_QUERY_STARTED) {
        sound->AddRef();   /* for the callback */
        snd_DoExternCall_CB(SUKR_RPC, SUKR_QUERY_PLAYING, sound->voiceHandle, 0, 0, 0, 0, OnStreamsndStatus, (long)sound);
        SetSndFlags(sound, SND_QUERY_STARTED, 0);
    }
    if (sound->playState)
        return;
    if (sound->flags & SND_QUERY_PLAYING) {
        sound->AddRef();
        snd_DoExternCall_CB(SUKR_RPC, SUKR_QUERY_POSITION, sound->voiceHandle, 0, 0, 0, 0, OnBankSoundStatus, (long)sound);
        SetSndFlags(sound, SND_QUERY_PLAYING, 0);
    }
    if ((sound->flags & SND_TIMED) && sound->pauseCount <= 0)   /* playing: advance the play time */
        sound->playTime += D_003DD040.tickNow - D_003DD040.tickLast;
}
} // namespace M_func_001FC878

/* STREAMSND slot 15: streams are not culled */
// Culling hook (slot 15): nothing, streams are never culled. (inferred)
extern "C" void SetStreamSoundCulled()
{
}

namespace M_UpdateStreamsndPositional {

extern qword D_002A2DD0 __asm__("D_002A2DD0");
int AreFloatsNear__Ffff(float,float,float) __asm__("AreFloatsNear__Ffff");
void UpdateSndPositional(CID151 *) __asm__("UpdateSndPositional");
qword GetLoWorldPos(LO *) __asm__("GetLoWorldPos");
void CalculateSndVolPan(CID151 *,qword,const LM *,float *,int *) __asm__("CalculateSndVolPan");
void SetStreamGroupPan(STREAMSND *,int,int) __asm__("SetStreamGroupPan");
void SetStreamsndGroupVolume(STREAMSND *,int,float) __asm__("SetStreamsndGroupVolume");
// True when a and b are equal within 0.0001 on x, y and z.
extern "C" { static inline int AreVectorsNear(qword a, qword b)
{
    return AreFloatsNear__Ffff(QwordX(a), QwordX(b), 0.0001f) && AreFloatsNear__Ffff(QwordY(a), QwordY(b), 0.0001f) && AreFloatsNear__Ffff(QwordZ(a), QwordZ(b), 0.0001f);
} }
// Positional update of a stream (slot 16): the base update, then per sub-group the loudest audible source sets the
// group's volume and pan.
// Also known as: Update3D (upstream readable / Sly 1 name).
extern "C" void UpdateStreamsndPositional(STREAMSND *sound)
{
    int group;
    if (sound->source || !AreVectorsNear(D_002A2DD0, sound->position))
        UpdateSndPositional(sound);
    for (group = 0; group < 6; group++) {
        int count = sound->groupSourceCounts[group];
        if (count) {
            float maximum = 0.0f;
            int panAngle = 0, numActive = 0, i;
            int *panOut = &panAngle;   /* only the first audible source's pan is computed */
            float fade;
            for (i = 0; i < count; i++) {
                SNDSOURCE *entry = &sound->groupSources[group][i];
                LO *object = entry->plo;
                if (object && (object->isInWorld & 1) == 1) {   /* in the world */
                    numActive++;
                    CalculateSndVolPan(sound, GetLoWorldPos(object), &entry->range, &fade, panOut);
                    panOut = 0;
                    { float gain = fade * entry->gain; maximum = maximum > gain ? maximum : gain; }
                }
            }
            SetStreamGroupPan(sound, group, numActive == 1 ? panAngle : 0);
            SetStreamsndGroupVolume(sound, group, maximum);
        }
    }
}
} // namespace M_UpdateStreamsndPositional

namespace M_func_001FCBA8 {
void ResumeSnd(CID151 *) __asm__("ResumeSnd");
// Async callback when a stream started: stores its stream handle, or stops the sound if starting failed; drops the
// callback's reference. (inferred)
extern "C" void OnStreamSoundStarted(int handle, long context)
{
    CID151 *sound = (CID151 *)context;
    sound->voiceHandle = handle;
    if (handle)
        SetSndFlags(sound, SND_QUERY_STARTED, SND_QUERY_STARTED);
    else {
        ResumeSnd(sound);
        sound->playState = 0;
        if (!(sound->flags & SND_STOP))
            sound->Start();
    }
    sound->Release();
}
} // namespace M_func_001FCBA8
#undef VF

namespace M_OnStreamsndStatus {
void ResumeSnd(CID151 *) __asm__("ResumeSnd");
// Async callback of the stream status query.
extern "C" void OnStreamsndStatus(int hasStopped, long context)
{
    CID151 *sound = (CID151 *)context;
    if (hasStopped) {
        sound->playState = 0;
        ResumeSnd(sound);
    } else {
        SetSndFlags(sound, SND_QUERY_STARTED, SND_QUERY_STARTED);   /* ask again */
    }
    sound->Release();
}
} // namespace M_OnStreamsndStatus

// Adds a reference to a streamed sound (slot 1).
void STREAMSND::AddRef()   /* key function: STREAMSND's vtable comes out here */
{
    refCount++;
}

namespace M_DecrefStreamsnd {
void BASIC_dtor(STREAMSND *, int) __asm__("_$_5BASIC");   /* ~BASIC with in-charge flag 0 */
void FreeStreamsnd(STREAMSND *) __asm__("FreeStreamsnd");
// Drops a reference to a streamed sound (slot 2); at zero it is destroyed and returned to its pool. (inferred)
extern "C" void DecrefStreamsnd(STREAMSND *sound)
{
    if (--sound->refCount == 0 && sound) {
        BASIC_dtor(sound, 0);
        FreeStreamsnd(sound);
    }
}
} // namespace M_DecrefStreamsnd

// Allocates a streamed sound from the manager's pool.
extern "C" void *AllocStreamsnd(int cb)
{
    return AllocZeroedFromSlotheap(&G_SNDMGR.streamSoundPool);
}

// Returns a streamed sound to the manager's pool.
extern "C" void FreeStreamsnd(void *pv)
{
    FreeSlotheapSlot(&G_SNDMGR.streamSoundPool, pv);
}

namespace M_SNDMGR_ctor {
struct V2 { float x, y; };
/* SNDMGR (layout: sly2/sndmgr.h): BASIC's header, the headset option, the bank / stream sound pools, the dirty list, the
   paused flag, the mute-when-stopped flags, the microphone noise curves (CLQ), the playback mode, the music layers'
   smoothing parameters, the counted sound pointers released by the destructor (dialog +0x200, the eight music-stack
   entries +0x208, ambience +0x228). */
extern const struct V2 D_002A83B8 __asm__("D_002A83B8");
extern const struct V2 D_002A83C0 __asm__("D_002A83C0");
void InitDl__FP2DLi(DL *, int) __asm__("InitDl__FP2DLi");
void FitClq__FffffP3CLQ(float, float, float, CLQ *, float) __asm__("FitClq__FffffP3CLQ");
// Initialise a smoothing record: fast speed and time, zero slow speed.
extern "C" { static inline void InitSmoothing(SMP *smoothing, float fastSpeed, float fastTime)
{
    smoothing->fastSpeed = fastSpeed;
    smoothing->slowSpeed = 0.0f;
    smoothing->fastDuration = fastTime;
} }
// Constructs the sound manager (g_sndmgr, at static initialisation): the bank and stream sound pools, empty counted
// pointers, the dirty list, the music layers' smoothing and the microphone noise curves.
SNDMGR *SNDMGR_ctor(SNDMGR *soundManager) __asm__("SNDMGR_ctor"); SNDMGR *SNDMGR_ctor(SNDMGR *soundManager)
{
    int n;
    int *entry;
    soundManager->scriptFrame.m_p = 0;   /* BASIC's construction as plain stores */
    soundManager->scriptVariables.m_p = 0;
    SLY2_SET_VPTR(soundManager, SNDMGR);
    InitSlotheap(&soundManager->bankSoundPool, sizeof(BANKSND), 0x80, 0x10);      /* 128 bank sounds (0x70 bytes on the EE) */
    InitSlotheap(&soundManager->streamSoundPool, sizeof(STREAMSND), 0x10, 0x10);   /* 16 stream sounds (0x3C0 bytes on the EE) */
    soundManager->dialogSound.p = 0;
    entry = (int *)soundManager->musicStack;
    for (n = 7; n != -1; n--) *entry++ = 0;
    soundManager->ambienceSound.p = 0;
    InitDl__FP2DLi(&soundManager->dirtySounds, SLY2_OFFSETOF(CID151, dleDirty));
    soundManager->muteWhenStoppedFlags = MUTE_WHEN_STOPPED_ALL;
    soundManager->isHeadsetEnabled = 0;
    soundManager->playbackMode = 0;
    InitSmoothing(&soundManager->musicLayerSmoothing[2], 1.0f, 0.25f);
    InitSmoothing(&soundManager->musicLayerSmoothing[1], 1.0f, 0.25f);
    {
        /* quadratics through (0, 0), (0.5, x), (1, y): noise range 500 / 3000 and height 500 / 1000 at half / full level */
        SMP *smoothing = &soundManager->musicLayerSmoothing[0];
        float zero;
        float half = 0.5f;
        InitSmoothing(smoothing, 3.0f, 0.25f);
        zero = smoothing->slowSpeed;
        FitClq__FffffP3CLQ(zero, D_002A83B8.y, half, &soundManager->micNoiseRangeCurve, D_002A83B8.x);
        FitClq__FffffP3CLQ(zero, D_002A83C0.y, half, &soundManager->micNoiseHeightCurve, D_002A83C0.x);
    }
    soundManager->isPaused = 0;
    return soundManager;
}
} // namespace M_SNDMGR_ctor

namespace M_StartupSound {
/* SNDMGR: the volume table (8 rows x 4 channels; row 2 is read as the stream channel volumes), the master volume,
   four channel volumes, this and the last update's tick */
extern SNDMGR D_003DD040 __asm__("D_003DD040");
void snd_StartSoundSystem(int) __asm__("snd_StartSoundSystem");
int snd_DoExternCall(int, int, int, int, int, int, int) __asm__("snd_DoExternCall");
long TickNow(struct CLOCK *) __asm__("TickNow");
// Boot-time sound setup: starts the 989snd system and the IOP stream module, sets all volumes to 1 and records the start
// time.
// Also known as: StartupGameSound (upstream readable / Sly 1 name; StartupGameSound__Fv).
extern "C" void StartupSound(void)
{
    int channel, row;
    long now;
    snd_StartSoundSystem(2);
    (void)snd_DoExternCall(SUKR_RPC, SUKR_INIT, 0, 0, 0, 0, 0);
    D_003DD040.masterVolume = 1.0f;
    for (channel = 0; channel < 4; channel++) {
        D_003DD040.channelVolumes[channel] = 1.0f;
        for (row = 0; row < 8; row++)
            D_003DD040.volumes[row][channel] = 1.0f;
    }
    now = TickNow(&g_clock);
    D_003DD040.tickNow = now;
    D_003DD040.tickLast = now;
}
} // namespace M_StartupSound

namespace M_func_001FCF70 {
// Async callback of the headset microphone level query (SUKR command 0xB, UpdateHeadsetMicrophone): stores the level and
// clears the busy flag. (inferred)
extern "C" void OnMicLevelResult(int result, long context) { SNDMGR *soundManager = (SNDMGR *)context; *(int *)&soundManager->micLevel = result;   /* the float's bits */ soundManager->isMicLevelQueryBusy = 0; }
} // namespace M_func_001FCF70

namespace M_func_001FCF88 {
// Async callback of the manager's periodic status query: stores the result and clears the busy flag. (inferred)
extern "C" void OnSoundManagerStatusResult(int result, long context) { SNDMGR *soundManager = (SNDMGR *)context; soundManager->statusResult = result; soundManager->isStatusQueryBusy = 0; }
} // namespace M_func_001FCF88

namespace M_func_001FCFA0 {
// The record of the dialog line with the given id (0 when the level has none).
// Also known as: PstreamFindDialog (upstream readable / Sly 1 name; PstreamFindDialog__FP8SOUNDMGRi).
SNDSTREAM *FindDialogStream(SNDMGR *soundManager, int id) __asm__("FindDialogStream"); SNDSTREAM *FindDialogStream(SNDMGR *soundManager, int id)
{
    SNDSTREAM *stream;
    for (stream = soundManager->firstDialogStream; stream; stream = stream->next)
        if (stream->id == id)
            return stream;
    return 0;
}
} // namespace M_func_001FCFA0

// Duration of a dialog line's stream by id (0 when unknown); subtitles use it to pace the typewriter effect. (inferred)
extern "C" float GetDialogStreamDuration(int id)
{
    SNDSTREAM *prec = FindDialogStream(id);
    float g = 0.0f;
    if (prec != 0) {
        g = prec->duration;
    }
    return g;
}

namespace M_func_001FCFF8 {
// The file record of the music track with the given id (0 when the level has none); PushMusic streams it.
// Also known as: PstreamFindMusic (upstream readable / Sly 1 name; PstreamFindMusic__FP8SOUNDMGRi).
extern "C" SNDFILE *FindMusicStream(SNDMGR *soundManager, int id)
{
    SNDFILE *file;
    for (file = soundManager->firstMusicFile; file; file = file->next)
        if (file->id == id)
            return file;
    return 0;
}
} // namespace M_func_001FCFF8

namespace M_PsndFromSfxid {
// The manager's sound entry for a sound-effect id, or 0 when the level does not have it.
// Also known as: IsoundFromSfxid (upstream readable / Sly 1 name; IsoundFromSfxid__FP8SOUNDMGRi).
extern "C" int *FindSoundForSfxId(SNDMGR *soundManager, int sfxId)
{
    int index;
    if (sfxId < 0 || sfxId >= soundManager->numSfx)
        return 0;
    index = soundManager->soundIndexFromSfx[sfxId];
    if (index < 0)
        return 0;
    return &soundManager->soundKinds[index];
}
} // namespace M_PsndFromSfxid

// Index of a sound entry in the manager's table.
extern "C" int GetSoundIndex(SNDMGR *soundManager, int *pn)
{
    return (int)((char *)pn - (char *)soundManager->soundKinds) >> 2;
}

namespace M_func_001FD080 {
// Which volume channel a stream belongs to: 1 for a music track, 0 for the ambience stream, 2 for anything else (the
// dialog streams). SetSndmgrChannelVolume confirms the mapping: channel 0 drives the ambience sound, 1 the music stack,
// 2 the dialog sound.
// Also known as: ItrackFromStream (upstream readable / Sly 1 name; ItrackFromStream__FP8SOUNDMGRP9SNDSTREAM).
extern "C" int GetStreamVolumeChannel(SNDMGR *soundManager, FILEREC *record)
{
    SNDFILE *file;
    for (file = soundManager->firstMusicFile; file; file = file->next)
        if (&file->record == record)
            return 1;
    return soundManager->ambienceRecord != record ? 2 : 0;
}
} // namespace M_func_001FD080

namespace M_func_001FD0C0 {
extern int D_002C3C40[] __asm__("D_002C3C40");
// OID of the state machine that animates dialog speaker role n (n < 6), or -1. (inferred)
extern "C" int GetDialogRoleSmOid(int unused, unsigned int role) { if (role >= 6) return -1; return D_002C3C40[role]; }
} // namespace M_func_001FD0C0

namespace M_LoadSoundSectionFromBrx {
// LoadSoundSectionFromBrx (0x1FD0E8), written from the sound section's layout (our names; Lilly's decomp has it as unmatched).
// Reads the level's sound section: the sound bank's file record (the loaded bank is kept when the record's file id is unchanged), per-sound kinds (S8),
// the effect id -> sound index table, the ambience stream record and its group ids (up to six), then the dialog stream records (one file record per
// language; the record of the language in g_nLang = D_002E1E54 is kept) and the music file records. The allocation requests are {cb, cbAlign} records
// built by inline helpers; every helper call has its own SWALLOC local, which the compiler gives the same stack slot (sp+0x40).
// Matched without pins or asm: the loops use `int *kind = &soundKinds[i]` (pointer formed before the read), a separate counter j for the index table,
// memset declared as returning void * (a void return moved a v0/v1 choice), and the ambience stream through a temp (no s-register held for &ambience).
typedef unsigned char    uchar;
typedef unsigned short   ushort;
typedef unsigned int     uint;
typedef uchar byte;
typedef unsigned long ulong;
void *PvAllocSwImpl(SWALLOC *pswalloc) __asm__("AllocFromWorldHeap");
void *PvAllocSwClearImpl(SWALLOC *pswalloc) __asm__("AllocClearedFromWorldHeap");
// Allocate cb bytes with alignment cbAlign from the world heap.
inline void *AllocWorldHeapBlock(int cb, int cbAlign)
{
    SWALLOC swalloc;
    swalloc.cb = cb;
    SWALLOC *pswalloc = &swalloc;
    pswalloc->cbAlign = cbAlign;
    return PvAllocSwImpl(pswalloc);
}
// Allocate cb zeroed bytes with alignment cbAlign from the world heap.
inline void *AllocClearedWorldHeapBlock(int cb, int cbAlign)
{
    SWALLOC swalloc;
    swalloc.cb = cb;
    SWALLOC *pswalloc = &swalloc;
    pswalloc->cbAlign = cbAlign;
    return PvAllocSwClearImpl(pswalloc);
}
struct STREAMSND;
extern int D_002E1E54 __asm__("D_002E1E54");
extern void snd_UnloadBank(int bank) __asm__("snd_UnloadBank");
extern int snd_FlushSoundCommands(void) __asm__("snd_FlushSoundCommands");
extern void *func_002333B4(void *, int, uint) __asm__("memset");
extern STREAMSND *PstreamsndNew(FILEREC *, int) __asm__("PstreamsndNew");
extern void AssignSoundRef(void *, STREAMSND *) __asm__("AssignSoundRef");
// True when two file records name the same file (same pointer, or equal key words 2 ^ 14).
inline int IsSameFileRecord(FILEREC *record, FILEREC *other)
{
    if (record == other)
        return 1;
    if (record == 0 || other == 0)
        return 0;
    return (record->aw[2] ^ record->aw[14]) == (other->aw[2] ^ other->aw[14]);
}
extern "C" void LoadSoundSectionFromBrx(SNDMGR *soundManager, CBinaryInputStream *input) __asm__("LoadSoundSectionFromBrx");
// Loads the level's sound section: the sound bank file, the kind of each sound, the effect-id table, the ambience
// stream and its groups, the dialog lines (current language) and the music tracks.
// Also known as: LoadMusicFromBrx (upstream readable / Sly 1 name; LoadMusicFromBrx__FP8SOUNDMGRP18CBinaryInputStream).
extern "C" void LoadSoundSectionFromBrx(SNDMGR *soundManager, CBinaryInputStream *input)
{
    FILEREC bankRecord;
    CBinaryInputStream_ReadCb64(input, &bankRecord);
    if (!IsSameFileRecord(&bankRecord, &soundManager->bankRecord))
    {
        soundManager->bankRecord = bankRecord;
        if (soundManager->bank)
        {
            snd_UnloadBank(soundManager->bank);
            while (snd_FlushSoundCommands())
                ;
            soundManager->bank = 0;
        }
    }
    int i;
    soundManager->numSounds = input->S16Read();
    soundManager->soundKinds = (int *)AllocClearedWorldHeapBlock(soundManager->numSounds * sizeof(int), 0x10);
    for (i = 0; i < soundManager->numSounds; i++)
    {
        int *kind = &soundManager->soundKinds[i];
        *kind = input->S8Read();
    }
    soundManager->numSfx = input->S16Read();
    soundManager->soundIndexFromSfx = (int *)AllocWorldHeapBlock(soundManager->numSfx * sizeof(int), 0x10);
    func_002333B4(soundManager->soundIndexFromSfx, -1, soundManager->numSfx * sizeof(int));
    int j;
    for (j = 0; j < soundManager->numSounds; j++)
        soundManager->soundIndexFromSfx[input->S16Read()] = j;
    soundManager->numAmbienceGroups = input->S16Read();
    if (soundManager->numAmbienceGroups > 0)
    {
        soundManager->ambienceRecord = (FILEREC *)AllocClearedWorldHeapBlock(sizeof(FILEREC), 0x10);
        CBinaryInputStream_ReadCb64(input, soundManager->ambienceRecord);
        STREAMSND *sound = PstreamsndNew(soundManager->ambienceRecord, 4);
        AssignSoundRef(&soundManager->ambienceSound, sound);
    }
    func_002333B4(soundManager->ambienceGroupIds, -1, sizeof(soundManager->ambienceGroupIds));
    for (i = 0; i < soundManager->numAmbienceGroups; i++)
        soundManager->ambienceGroupIds[i] = input->S16Read();
    int numLanguages = input->U8Read();
    int currentLanguage = 0;
    for (int language = 0; language < numLanguages; language++)
    {
        if (input->U8Read() == D_002E1E54)
            currentLanguage = language;
    }
    int numDialogStreams = input->S32Read();
    SNDSTREAM *dialogStreams = (SNDSTREAM *)AllocClearedWorldHeapBlock(numDialogStreams * sizeof(SNDSTREAM), 0x10);
    for (i = 0; i < numDialogStreams; i++)
    {
        SNDSTREAM *dialogStream = &dialogStreams[i];
        dialogStream->id = input->S16Read();
        dialogStream->speakerId = input->S16Read();
        for (int language = 0; language < numLanguages; language++)
        {
            FILEREC record;
            CBinaryInputStream_ReadCb64(input, &record);
            int excludeFromHeadset = input->S8Read();
            float duration = input->F32Read();
            if (language == currentLanguage)
            {
                dialogStream->record = record;
                dialogStream->excludeFromHeadset = excludeFromHeadset;
                dialogStream->duration = duration;
            }
        }
        dialogStream->numEvents = input->S32Read();
        dialogStream->events = (DIALOGEVENT *)AllocWorldHeapBlock(dialogStream->numEvents * 8, 0x10);
        input->Read(dialogStream->numEvents * 8, dialogStream->events);
        dialogStream->next = soundManager->firstDialogStream;
        soundManager->firstDialogStream = dialogStream;
    }
    int numMusicFiles = input->S32Read();
    SNDFILE *musicFiles = (SNDFILE *)AllocClearedWorldHeapBlock(numMusicFiles * sizeof(SNDFILE), 0x10);
    for (i = 0; i < numMusicFiles; i++)
    {
        SNDFILE *musicFile = &musicFiles[i];
        musicFile->id = input->S16Read();
        CBinaryInputStream_ReadCb64(input, musicFile);
        musicFile->next = soundManager->firstMusicFile;
        soundManager->firstMusicFile = musicFile;
    }
}
} // namespace M_LoadSoundSectionFromBrx

namespace M_PostSoundLoad {
struct K { int keyLo; int keyHi; int kind; char filename; char pad[0x110 - 0xD]; };   /* a file lookup key */
void InitFileKey(struct K *) __asm__("InitFileKey");
int snd_BankLoadByLoc(int, int) __asm__("snd_BankLoadByLoc");
void snd_ResolveBankXREFS(void) __asm__("snd_ResolveBankXREFS");
int snd_IsSoundALooper(int, int) __asm__("snd_IsSoundALooper");
/* SOUND_KIND_LOOPING: defined with PlayBanksnd */
extern "C" { static inline int BuildStreamFileKey(struct K *key, FILEREC *record)
{
    if (!record) return 0;
    {
        int k2 = record->aw[13];   /* the key words at +0x34, +0x0, +0x3C, +0x4 */
        int k0 = record->aw[0];
        int k3 = record->aw[15];
        int k1 = record->aw[1];
        key->kind = 2;
        key->keyHi = k1 ^ k3;
        key->keyLo = (k0 ^ k2) + 0xCA0;
    }
    return key->keyHi != 0;
} }
// After loading: loads the sound bank if needed, resolves its cross references and marks which sounds loop.
// Also known as: LoadSoundBank (upstream readable / Sly 1 name; LoadSoundBank__FP8SOUNDMGR).
extern "C" void PostSoundLoad(SNDMGR *soundManager)
{
    int i;
    if (!soundManager->bank) {
        struct K key;
        key.kind = 2;
        InitFileKey(&key);
        if (BuildStreamFileKey(&key, &soundManager->bankRecord) && key.kind == 2)
            soundManager->bank = snd_BankLoadByLoc(key.keyLo, 0);
        snd_ResolveBankXREFS();
    }
    for (i = 0; i < soundManager->numSounds; i++) {
        if (soundManager->soundKinds[i] == SOUND_KIND_LOOPING)
            continue;
        if (snd_IsSoundALooper(soundManager->bank, i))
            soundManager->soundKinds[i] = SOUND_KIND_LOOPING;
    }
}
} // namespace M_PostSoundLoad

namespace M_ResetSndmgr {
/* SNDMGR (layout: sly2/sndmgr.h) */
extern qword D_002A2DD0 __asm__("D_002A2DD0");
extern char D_004435F0[] __asm__("D_004435F0");
void snd_StopAllStreams(void) __asm__("snd_StopAllStreams");
void snd_DoExternCall(int, int, int, int, int, int, int) __asm__("snd_DoExternCall");
int snd_FlushSoundCommands(void) __asm__("snd_FlushSoundCommands");
void ReplaceHeldNoise(int, qword, int, void *) __asm__("ReplaceHeldNoise");
void *func_002333B4(void *, int, int) __asm__("memset");
void ClearSubtitle(void *) __asm__("ClearSubtitle");
void AssignSoundRef(void *, void *) __asm__("AssignSoundRef");
void ResetSlotheap(void *) __asm__("ResetSlotheap");
void ClearDl__FP2DL(void *) __asm__("ClearDl__FP2DL");
void ResetSndmgrVolumes(SNDMGR *) __asm__("ResetSndmgrVolumes");
// Stops every sound and stream, flushes the hardware queues, clears the manager's state and dirty list and resets volumes
// (level teardown).
// Also known as: ResetSoundMgr (upstream readable / Sly 1 name; ResetSoundMgr__FP8SOUNDMGR).
extern "C" void ResetSndmgr(SNDMGR *soundManager)
{
    snd_StopAllStreams();
    snd_DoExternCall(SUKR_RPC, SUKR_STOP_ALL, 0, 0, 0, 0, 0);
    while (snd_FlushSoundCommands())
        ;
    soundManager->unknown14 = 0;
    soundManager->numSfx = -1;
    soundManager->numAmbienceGroups = 0;
    soundManager->musicLayer2Requests = 0;
    soundManager->musicLayer0Requests = 0;
    soundManager->sfxVariationStep = 0;
    soundManager->firstDialogStream = 0;
    soundManager->firstMusicFile = 0;
    soundManager->ambienceRecord = 0;
    soundManager->numSounds = 0;
    soundManager->soundKinds = 0;
    soundManager->soundIndexFromSfx = 0;
    ReplaceHeldNoise(0, D_002A2DD0, 0, &soundManager->micNoise);   /* release the microphone noise */
    func_002333B4(soundManager->musicLayerWeights, 0, sizeof(soundManager->musicLayerWeights));
    ClearSubtitle(D_004435F0);
    AssignSoundRef(&soundManager->dialogSound, 0);
    soundManager->musicStackCount = 0;
    func_002333B4(soundManager->musicStack, 0, sizeof(soundManager->musicStack));
    AssignSoundRef(&soundManager->ambienceSound, 0);
    func_002333B4(soundManager->dialogRoleObjects, 0, sizeof(soundManager->dialogRoleObjects));
    soundManager->muteWhenStoppedFlags = MUTE_WHEN_STOPPED_ALL;
    ResetSlotheap(&soundManager->bankSoundPool);
    ResetSlotheap(&soundManager->streamSoundPool);
    ClearDl__FP2DL(&soundManager->dirtySounds);
    ResetSndmgrVolumes(soundManager);
}
} // namespace M_ResetSndmgr

namespace M_UpdateSndmgr {
struct K { float fastSpeed, slowSpeed, fastTime; };   /* SMP */
extern int g_cds __asm__("g_cds");
extern struct K D_002A8288 __asm__("D_002A8288");
long TickNow(CLOCK *) __asm__("TickNow");
void UpdateHeadsetMicrophone(SNDMGR *) __asm__("UpdateHeadsetMicrophone");
void PollSoundManagerStatus(SNDMGR *) __asm__("PollSoundManagerStatus");
void ResumeSnd(CID151 *) __asm__("ResumeSnd");
void PauseSnd(CID151 *) __asm__("PauseSnd");
void UpdateMusicMix(SNDMGR *) __asm__("UpdateMusicMix");
void UpdateSoundManagerStub(SNDMGR *) __asm__("UpdateSoundManagerStub");
void UpdateDialog(SNDMGR *) __asm__("UpdateDialog");
int ShouldFadeWhenTimeStops(SNDMGR *) __asm__("ShouldFadeWhenTimeStops");
float SmoothValue__FfffP3SMPPf(float, float, float, struct K *, float *) __asm__("SmoothValue__FfffP3SMPPf");
void SetSndmgrChannelVolume(SNDMGR *, int, float, int) __asm__("SetSndmgrChannelVolume");   /* (manager, channel, volume, category): the float travels in $f12 */
void UpdateSnd(CID151 *) __asm__("UpdateSnd");
int snd_FlushSoundCommands(void) __asm__("snd_FlushSoundCommands");
// Per-frame sound update: timing, IOP status polls, global pause while the game is paused, music mix, dialog events,
// master fades, then flushes every dirty sound and the 989snd command queue.
// Also known as: UpdateSoundMgr (upstream readable / Sly 1 name; UpdateSoundMgr__FPv).
extern "C" void UpdateSndmgr(SNDMGR *soundManager)
{
    STREAMSND *music;
    float fadeVolume, targetVolume;
    struct K smoothing;
    CID151 *sound;
    soundManager->tickLast = soundManager->tickNow;
    soundManager->tickNow = TickNow(&g_clock);
    UpdateHeadsetMicrophone(soundManager);
    PollSoundManagerStatus(soundManager);
    music = soundManager->musicStackCount > 0 ? (soundManager->musicStack + soundManager->musicStackCount - 1)->p : 0;
    /* the long-running sounds stay paused while the disc is busy (inferred: disc errors / reads) */
    if (g_cds == CD_STATE_IDLE) {
        if (soundManager->isPaused) {
            if (music) ResumeSnd(music);
            if (soundManager->ambienceSound) ResumeSnd(soundManager->ambienceSound);
            if (soundManager->dialogSound) ResumeSnd(soundManager->dialogSound);
            soundManager->isPaused = 0;
        }
    } else if (!soundManager->isPaused) {
        if (music) PauseSnd(music);
        if (soundManager->ambienceSound) PauseSnd(soundManager->ambienceSound);
        if (soundManager->dialogSound) PauseSnd(soundManager->dialogSound);
        soundManager->isPaused = 1;
    }
    targetVolume = 1.0f;
    UpdateMusicMix(soundManager);
    UpdateSoundManagerStub(soundManager);
    UpdateDialog(soundManager);
    smoothing = D_002A8288;
    /* game time stopped (inferred: the pause menu): fade the ambience and music out, cut the dialog */
    if (ShouldFadeWhenTimeStops(soundManager) && g_clock.deltaTime == 0.0f) targetVolume = 0.0f;
    fadeVolume = SmoothValue__FfffP3SMPPf(soundManager->volumes[VOLUME_CATEGORY_PAUSE_FADE][VOLUME_CHANNEL_AMBIENCE], targetVolume, g_clock.realDeltaTime, &smoothing, 0);
    SetSndmgrChannelVolume(soundManager, VOLUME_CHANNEL_AMBIENCE, fadeVolume, VOLUME_CATEGORY_PAUSE_FADE);
    SetSndmgrChannelVolume(soundManager, VOLUME_CHANNEL_MUSIC, fadeVolume, VOLUME_CATEGORY_PAUSE_FADE);
    SetSndmgrChannelVolume(soundManager, VOLUME_CHANNEL_DIALOG, targetVolume, VOLUME_CATEGORY_PAUSE_FADE);
    for (sound = (CID151 *)soundManager->dirtySounds.head; sound;) {
        CID151 *next = (CID151 *)sound->dleDirty.next;
        UpdateSnd(sound);
        sound = next;
    }
    snd_FlushSoundCommands();
}
} // namespace M_UpdateSndmgr

namespace M_func_001FDA48 {
/* NOISETYPE (noise.cpp): range +0x54, height range +0x58 (low, high) */
struct O { char pad[0x54]; float range; float heightLow; float heightHigh; };
void OnMicLevelResult(void) __asm__("OnMicLevelResult");
void snd_DoExternCall_CB(int, int, int, int, int, int, int, void (*)(void), long) __asm__("snd_DoExternCall_CB");
void *GetHeroPo(SW *) __asm__("GetHeroPo");
int IsBasicDerivedFrom__FP5BASIC3CID(void *, int) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
qword GetLoWorldPos(void *) __asm__("GetLoWorldPos");
void ReplaceHeldNoise(struct O *, qword, void *, void *) __asm__("ReplaceHeldNoise");
#define CID_STEPPLAYER 0x34
// Evaluate the quadratic c0 + c1*t + c2*t^2.
extern "C" { static inline float Eval(CLQ *curve, float t)   /* g0 + g1 t + g2 t^2 */
{
    return curve->g0 + t * (curve->g1 + t * curve->g2);
} }
// Headset microphone: with the headset option on, keeps one level query (SUKR command 0xB) in flight; while the hero is
// a player and the level is above half, the hero makes a noise guards can hear, its range and height growing with the
// level (ReplaceHeldNoise; with no noise type the held noise is released). (inferred; was named UpdateSoundListener,
// UpdateSndmgrListener in the draft annotation)
extern "C" void UpdateHeadsetMicrophone(SNDMGR *soundManager)
{
    struct O *noiseType;
    qword position;
    void *hero;
    if (!soundManager->isHeadsetEnabled) {
        soundManager->micLevel = 0.0f;
    } else if (!soundManager->isMicLevelQueryBusy) {
        soundManager->isMicLevelQueryBusy = 1;
        snd_DoExternCall_CB(SUKR_RPC, SUKR_QUERY_MIC_LEVEL, 0, 0, 0, 0, 0, OnMicLevelResult, (long)soundManager);
    }
    noiseType = 0;
    hero = GetHeroPo(&g_sw);
    if (hero && IsBasicDerivedFrom__FP5BASIC3CID(hero, CID_STEPPLAYER) && 0.5f < soundManager->micLevel) {
        float height;
        noiseType = (O *)g_sw.apstimk[5];
        position = GetLoWorldPos(hero);
        noiseType->range = Eval(&soundManager->micNoiseRangeCurve, soundManager->micLevel);
        height = Eval(&soundManager->micNoiseHeightCurve, soundManager->micLevel);
        noiseType->heightHigh = height;
        noiseType->heightLow = -height;
    }
    ReplaceHeldNoise(noiseType, position, hero, soundManager->micNoise);
}
} // namespace M_func_001FDA48

namespace M_func_001FDBA8 {
void OnSoundManagerStatusResult(void) __asm__("OnSoundManagerStatusResult");
void snd_DoExternCall_CB(int id, int n, int a, int b, int c, int d, int e, void (*pfn)(void), long p) __asm__("snd_DoExternCall_CB");
// Issues the manager's periodic status query to the IOP, once at a time. (inferred)
extern "C" void PollSoundManagerStatus(SNDMGR *soundManager)
{
    if (!soundManager->isStatusQueryBusy) {
        soundManager->isStatusQueryBusy = 1;
        snd_DoExternCall_CB(SUKR_RPC, SUKR_QUERY_STATUS, 0, 0, 0, 0, 0, OnSoundManagerStatusResult, (long)soundManager);
    }
}
} // namespace M_func_001FDBA8

namespace M_func_001FDC08 {
// Select and normalize three animation blend weights.

struct PLAYER {char pad4C8[0x4C8];int action;char pad4CC[0xBFC];int mode;char pad10CC[0x20];int state;};
/* SNDMGR (layout: sly2/sndmgr.h) */
struct PLAYER *GetCurrentPo(void *) __asm__("GetCurrentPo");
int IsBasicDerivedFrom__FP5BASIC3CID(void *,int) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
float SmoothValue__FfffP3SMPPf(float,float,float,SMP *,float *) __asm__("SmoothValue__FfffP3SMPPf");
void SetStreamsndGroupVolume(STREAMSND *,int,float) __asm__("SetStreamsndGroupVolume");
// Picks the music layer (0, 1 or 2) from requests and the player's state (alert, hiding, actions) and cross-fades the
// three layers of the current music stream toward it, keeping their total power constant. (inferred)
// Also known as: UpdateStemWeights (upstream readable / Sly 1 name; UpdateStemWeights__FPv).
extern "C" void UpdateMusicMix(SNDMGR *input)
{
    SNDMGR *soundManager = input;
    STREAMSND *music;
    int i;
    float sum, selectedSquare;
    soundManager->selectedMusicLayer = 1;
    if (soundManager->musicLayer2Requests) soundManager->selectedMusicLayer = 2;
    else if (soundManager->musicLayer0Requests) soundManager->selectedMusicLayer = 0;
    else if (GetCurrentPo(&g_sw) && IsBasicDerivedFrom__FP5BASIC3CID(GetCurrentPo(&g_sw), 0x34)) {
        struct PLAYER *player = GetCurrentPo(&g_sw);
        if (player->state == 3) soundManager->selectedMusicLayer = 0;
        else if (player->state == 4) soundManager->selectedMusicLayer = 2;
        else if (player->mode != -1) soundManager->selectedMusicLayer = 0;
        else if (player->action >= 0) { if (player->action < 3 || player->action == 13) soundManager->selectedMusicLayer = 0; }
    }
    /* the top of the music stack (this form matches; musicStack[count - 1] does not) */
    if (soundManager->musicStackCount > 0) music = ((STREAMSND **)&soundManager->musicStackCount)[soundManager->musicStackCount];
    else music = 0;
    /* a stream that is still starting jumps straight to the selected layer, a playing one cross-fades (the goto shape
       is the one that matches: the smoothing call is emitted after the jump) */
    if (music) {
        int isStarting = 0;
        if (music->playState || (music->flags & SND_START)) isStarting = 1;
        if (!isStarting) goto smooth;
    }
    soundManager->musicLayerWeights[soundManager->selectedMusicLayer] = 1.0f;
    goto normalize;
smooth:
    soundManager->musicLayerWeights[soundManager->selectedMusicLayer] = SmoothValue__FfffP3SMPPf(soundManager->musicLayerWeights[soundManager->selectedMusicLayer],
        1.0f, g_clock.realDeltaTime, &soundManager->musicLayerSmoothing[soundManager->selectedMusicLayer], 0);
normalize:
    /* scale the other layers so the squares of the weights sum to 1 (constant power) */
    sum = 0.0f;
    selectedSquare = soundManager->musicLayerWeights[soundManager->selectedMusicLayer] * soundManager->musicLayerWeights[soundManager->selectedMusicLayer];
    for (i = 0; i < 3; i++)
        if (i != soundManager->selectedMusicLayer) sum += soundManager->musicLayerWeights[i] * soundManager->musicLayerWeights[i];
    if (sum < 0.0001f) {
        for (i = 0; i < 3; i++) {
            soundManager->musicLayerWeights[i] = (i == soundManager->selectedMusicLayer) ? 1.0f : 0.0f;
        }
    } else {
        float gain = SqrtFloat((1.0f - selectedSquare) / sum);
        for (i = 0; i < 3; i++)
            if (i != soundManager->selectedMusicLayer) soundManager->musicLayerWeights[i] *= gain;
    }
    if (music) {
        SetStreamsndGroupVolume(music, 0, soundManager->musicLayerWeights[0]);
        SetStreamsndGroupVolume(music, 1, soundManager->musicLayerWeights[1]);
        SetStreamsndGroupVolume(music, 2, soundManager->musicLayerWeights[2]);
    }
}


} // namespace M_func_001FDC08

// Empty per-frame hook in the release build. (inferred)
extern "C" void UpdateSoundManagerStub()
{
}

namespace M_func_001FDEC8 {
typedef unsigned long ulong;
void StopDialog(SNDMGR *) __asm__("StopDialog");
void ApplyDialogEvent(SNDMGR *, DIALOGEVENT *) __asm__("ApplyDialogEvent");
// Advances the current dialog: fires its lip-sync / animation events as the stream's play time passes them and stops the
// dialog at its end. (inferred)
// Also known as: UpdateDialogEvents (upstream readable / Sly 1 name; UpdateDialogEvents__FPv).
extern "C" void UpdateDialog(SNDMGR *soundManager)
{
    STREAMSND *dialog = soundManager->dialogSound;
    SNDSTREAM *record;
    int hasEnded;
    if (!dialog) return;
    {
        int isStarting = dialog->playState || (dialog->flags & SND_START);
        if (isStarting) return;
    }
    dialog = soundManager->dialogSound;
    if (dialog->pauseCount > 0) return;
    record = dialog->record.psndstream;
    soundManager->dialogElapsed += g_clock.realDeltaTime;
    /* no voice and not starting: the stream is over (or never started) */
    hasEnded = 0;
    if (!dialog->voiceHandle) {
        hasEnded = dialog->playState || (dialog->flags & SND_START);
        hasEnded ^= 1;
    }
    if (hasEnded) {
        if (0.0f < (float)soundManager->dialogSound->playTime / PLAY_TIME_TICKS_PER_SECOND || record->duration <= soundManager->dialogElapsed)
            StopDialog(soundManager);
        return;
    }
    /* fire every event whose time (frames at 60 per second, relative to the previous one) the stream has reached */
    while (soundManager->dialogEventIndex < record->numEvents) {
        DIALOGEVENT *event = &record->events[soundManager->dialogEventIndex];
        float delay = (float)event->framesAfterPrevious * 0.016666668f;
        if ((float)soundManager->dialogSound->playTime / PLAY_TIME_TICKS_PER_SECOND < soundManager->dialogEventTime + delay) return;
        ApplyDialogEvent(soundManager, event);
        soundManager->dialogEventTime += delay;
        soundManager->dialogEventIndex++;
    }
}
} // namespace M_func_001FDEC8

namespace M_func_001FE0E8 {
#define DIALOG_ROLE_SPEAKER 3   /* the speaking object itself; the other roles are its state machines */
void SetSoEyesClosed(void *, float) __asm__("SetSoEyesClosed");
void SetSmaGoal(void *, int) __asm__("SetSmaGoal");
// Applies one dialog event: a gesture for role 3, otherwise sets the goal state of the speaker role's state machine
// (lip sync). (inferred)
// Also known as: FireDialogTarget (upstream readable / Sly 1 name; FireDialogTarget__FP8SOUNDMGRPv).
extern "C" void ApplyDialogEvent(SNDMGR *soundManager, DIALOGEVENT *event)
{
    int role = event->role;
    if (role != -1 && soundManager->dialogRoleObjects[role]) {
        if (role == DIALOG_ROLE_SPEAKER) SetSoEyesClosed(soundManager->dialogRoleObjects[DIALOG_ROLE_SPEAKER], event->value.gesture);
        else if (event->value.smState != -1) SetSmaGoal(soundManager->dialogRoleObjects[role], event->value.smState);
    }
}
} // namespace M_func_001FE0E8

namespace M_PushMusic {
extern LO *D_002DD5BC __asm__("D_002DD5BC");   /* Bentley */
extern LO *D_002F7900 __asm__("D_002F7900");   /* Murray */
extern LO *D_002E1E40 __asm__("D_002E1E40");   /* Sly (g_pjt) */

SNDREFC FindMusic(SNDMGR *, int) __asm__("FindMusic");
STREAMSND *PstreamsndNew(FILEREC *, int) __asm__("PstreamsndNew");
FILEREC *FindMusicStream(SNDMGR *, int) __asm__("FindMusicStream");
void AddStreamGroupSource(STREAMSND *, int, LO *, float, float *) __asm__("AddStreamGroupSource");   /* (sound, group, object, gain, range): the float travels in $f12 */

struct RANGE { float near, far; };
// Set a range's near and far distances.
static inline void Set(RANGE *range, float near, float far) { range->near = near; range->far = far; }
#define STREAM_KIND_LOOPING 4   /* STREAMSND kind bit 2 (GetStreamSoundKind) */
#define MUSIC_STACK_DEPTH 8
// Starts a music track: stops the current one, then either restarts the track if it is already on the stack (moving it to
// the top) or creates a looping stream for it whose sub-groups 3 and 4 follow Bentley / Murray and Sly (positional
// layers, 2000..5000 units), and pushes it on the 8-deep music stack. Returns the stream (0: unknown id or full stack).
// Also known as: PsndPushMusic (upstream readable / Sly 1 name; PsndPushMusic__FP8SOUNDMGRi).
extern "C" STREAMSND *PushMusic(SNDMGR *soundManager, int id)
{
    SNDREFC music = FindMusic(soundManager, id);
    STREAMSND *current = soundManager->musicStackCount > 0 ? (soundManager->musicStack + soundManager->musicStackCount - 1)->p : 0;
    if (current) StopSnd(current, 0.0f);
    if (music.p) {
        music->Start();
    } else {
        FILEREC *file = FindMusicStream(soundManager, id);
        if (!file || soundManager->musicStackCount == MUSIC_STACK_DEPTH) return 0;
        float gain = 1.0f;
        music = PstreamsndNew(file, STREAM_KIND_LOOPING);
        RANGE range;
        Set(&range, 2000.0f, 5000.0f);
        AddStreamGroupSource(music.p, 3, D_002DD5BC, gain, &range.near);
        AddStreamGroupSource(music.p, 3, D_002F7900, gain, &range.near);
        AddStreamGroupSource(music.p, 4, D_002E1E40, gain, &range.near);
    }
    return (soundManager->musicStack[soundManager->musicStackCount++] = music).p;
}
} // namespace M_PushMusic

namespace M_func_001FE310 {
extern void FindMusic(SNDREF *, SNDMGR *, int) SLY2_SRET_LABEL("FindMusic", "FindMusicInto");   /* its by-value result, by hand */
// Stops a music track by id: takes it off the stack (FindMusic) and, unless it had already stopped, stops it and
// restarts the track now on top of the stack. (inferred)
// Also known as: PopMusic (upstream readable / Sly 1 name; PopMusic__FP8SOUNDMGRi).
extern "C" void StopMusic(SNDMGR *soundManager, int id)
{
    SNDREF music;
    FindMusic(&music, soundManager, id);
    if (music.p) {
        /* stopped: no voice and not starting */
        int isStopped = 0;
        if (!music.p->voiceHandle) { isStopped = music.p->playState || (music.p->flags & SND_START); isStopped ^= 1; }
        if (!isStopped) {
            StopSnd(music.p, 0.0f);
            AssignSoundRef(&music, soundManager->musicStackCount > 0 ? (soundManager->musicStack + soundManager->musicStackCount - 1)->p : 0);
            if (music.p) music.p->Start();
        }
    }
}
} // namespace M_func_001FE310

namespace M_TMusicCur {
typedef unsigned long ulong;
// Play time of the current music track in seconds.
// Also known as: TMusicTop (upstream readable / Sly 1 name; TMusicTop__FPv).
extern "C" float GetMusicTime(SNDMGR *soundManager)
{
    STREAMSND *music;
    if (soundManager->musicStackCount > 0) { music = *(soundManager->musicStack + soundManager->musicStackCount - 1); }
    else music = 0;
    if (!music) return 0.0f;
    return (float)music->playTime * (1.0f / PLAY_TIME_TICKS_PER_SECOND);
}
} // namespace M_TMusicCur

namespace M_func_001FE478 {
extern FILEREC *FindMusicStream(SNDMGR *, int) __asm__("FindMusicStream");
// Takes the stream of the music track with the given id off the music stack and returns a reference to it (null when
// the track is not on the stack); the entries above it move down. (inferred)
// Also known as: RefTakeMusic (upstream readable / Sly 1 name; RefTakeMusic__FP8SOUNDMGRi).
extern "C" SNDREFC FindMusic(SNDMGR *soundManager, int id)
{
    FILEREC *file = FindMusicStream(soundManager, id);
    int i;
    for (i = 0; i < soundManager->musicStackCount; i++)
        if (soundManager->musicStack[i]->record.pfilerec == file) break;
    if (i == soundManager->musicStackCount) return SNDREFC(0);
    SNDREFC music(soundManager->musicStack[i]);
    soundManager->musicStackCount--;
    for (; i < soundManager->musicStackCount; i++) soundManager->musicStack[i] = soundManager->musicStack[i + 1];
    return music;
}
} // namespace M_func_001FE478

namespace M_func_001FE5B8 {
// Sets or clears bits of the manager's mute-when-stopped flags (bit 1: a player mode, bit 2: cutscenes clear it while
// they run, so stopping game time then keeps the music and ambience audible). (inferred)
// Also known as: SetSoundMgrFlags (upstream readable / Sly 1 name; SetSoundMgrFlags__FP8SOUNDMGRUii).
extern "C" void SetSoundManagerFlags(SNDMGR *soundManager, int mask, int isSet) { if (isSet) soundManager->muteWhenStoppedFlags |= mask; else soundManager->muteWhenStoppedFlags &= ~mask; }
} // namespace M_func_001FE5B8

// True when both mute-when-stopped flags are set (the default): UpdateSndmgr then fades the ambience and music out
// while game time is stopped (the pause menu). Cutscenes and a player mode clear a flag to keep them playing.
extern "C" int ShouldFadeWhenTimeStops(SNDMGR *soundManager)
{
    return soundManager->muteWhenStoppedFlags == MUTE_WHEN_STOPPED_ALL;
}

namespace M_StartDialog {
extern char D_004435F0[] __asm__("D_004435F0");

void StopDialog(SNDMGR *, int) __asm__("StopDialog");
SNDSTREAM *FindDialogStream(SNDMGR *, int) __asm__("FindDialogStream");
STREAMSND *PstreamsndNew(FILEREC *, int) __asm__("PstreamsndNew");
void SetSubtitle(void *, int, int) __asm__("SetSubtitle");
BASIC *FindSwObjectByClass__FP2SWi3CIDP2LO(SW *, int, int, LO *ploContext) __asm__("FindSwObjectByClass__FP2SWi3CIDP2LO");
int GetDialogRoleSmOid(SNDMGR *, int) __asm__("GetDialogRoleSmOid");
BASIC *FindLiveSma(BASIC *, int) __asm__("FindLiveSma");

// Starts a dialog line: stops the current one, creates its stream, shows its subtitle (unless noSubtitle) and, when the
// line has events and a speaker, binds the six lip-sync roles: role 3 is the speaker object, the others the state
// machines GetDialogRoleSmOid names on it. Each bound object is counted (vtable slot 1). Returns the stream.
// Also known as: PsndStartDialog (upstream readable / Sly 1 name; PsndStartDialog__FP8SOUNDMGRii).
extern "C" STREAMSND *StartDialog(SNDMGR *soundManager, int id, int noSubtitle)
{
    SNDSTREAM *record;
    StopDialog(soundManager, id);
    record = FindDialogStream(soundManager, id);
    if (!record) return 0;
    soundManager->dialogSound = PstreamsndNew(&record->record, 0);
    soundManager->dialogEventIndex = 0;
    soundManager->dialogEventTime = 0;
    soundManager->dialogElapsed = 0;
    if (!noSubtitle) SetSubtitle(D_004435F0, id, record->speakerId);
    if (record->speakerId != -1 && record->numEvents) {
        register BASIC *speaker __asm__("$19")=FindSwObjectByClass__FP2SWi3CIDP2LO(&g_sw, 5, record->speakerId, 0);
        if (speaker) {
            register int role __asm__("$18")=0;
            register int speakerRole __asm__("$20")=3;
            register BASIC **slot __asm__("$16")=soundManager->dialogRoleObjects;
            for(;role<6;role++,slot++) {
                if(role==speakerRole)soundManager->dialogRoleObjects[3]=speaker;
                else *slot=FindLiveSma(speaker,GetDialogRoleSmOid(soundManager,role));
                { BASIC *current=*slot;
                  if(current) current->AddRef(); }
            }
        }
    }
    return soundManager->dialogSound;
}
} // namespace M_StartDialog

namespace M_StopDialog {
#define SM_STATE_MOUTH_REST 0x611   /* (inferred: the speaker's idle mouth) */
void ApplyDialogEvent(SNDMGR *, DIALOGEVENT *) __asm__("ApplyDialogEvent");
void SetSmaGoal(void *, int) __asm__("SetSmaGoal");
void func_002333B4(void *, int, int) __asm__("memset");
// Stop the referenced sound and drop the reference, if any.
extern "C" { static inline void Stop(SNDREF *ref)
{
    if (ref && ref->p) {
        StopSnd(ref->p, 0.0f);
        AssignSoundRef(ref, 0);
    }
} }
// Stops the current dialog: flushes its remaining events, returns the speakers' mouths to rest, stops the stream and
// clears the dialog state.
// Also known as: StopDialog (upstream readable / Sly 1 name; StopDialog__FP8SOUNDMGR).
extern "C" void StopDialog(SNDMGR *soundManager)
{
    int role;
    SNDSTREAM *record;
    if (!soundManager->dialogSound.p) return;
    record = soundManager->dialogSound.p->record.psndstream;
    while (soundManager->dialogEventIndex < record->numEvents) {
        ApplyDialogEvent(soundManager, &record->events[soundManager->dialogEventIndex]);
        soundManager->dialogEventIndex++;
    }
    if (soundManager->dialogRoleObjects[5]) SetSmaGoal(soundManager->dialogRoleObjects[5], SM_STATE_MOUTH_REST);
    Stop(&soundManager->dialogSound);
    soundManager->dialogEventIndex = 0;
    soundManager->dialogEventTime = 0;
    soundManager->dialogElapsed = 0.0f;
    for (role = 0; role < 6; role++) {
        BASIC *object = soundManager->dialogRoleObjects[role];
        if (object) object->Release();
    }
    func_002333B4(soundManager->dialogRoleObjects, 0, sizeof(soundManager->dialogRoleObjects));
}
} // namespace M_StopDialog

namespace M_func_001FE848 {
// The constant 1 materialised in $4.
extern "C" { static inline int PresentValue(void){register int r __asm__("$4");__asm__("addiu %0,$zero,%1":"=r"(r):"i"(1));return r;} }
// True while a dialog (optionally the one with the given id) is playing. (inferred)
// Also known as: FDialogPlaying (upstream readable / Sly 1 name; FDialogPlaying__FP8SOUNDMGRi).
extern "C" int IsDialogPlaying(SNDMGR *soundManager, int id)
{
    SNDSTREAM *record; int isPlaying;
    if (soundManager->dialogSound) {
        record = soundManager->dialogSound->record.psndstream;
        isPlaying = 0;
        if (id == -1 || record->id == id) isPlaying = PresentValue();
        return isPlaying;
    }
    return 0;
}

typedef char guard_int_size[(sizeof(int)==4)?1:-1];
typedef char guard_pointer_size[(sizeof(void*)==4)?1:-1];
} // namespace M_func_001FE848

namespace M_func_001FE888 {
typedef unsigned long ulong;
// The sound's play time in seconds.
extern "C" { static inline float PlaySeconds(STREAMSND *sound) { return (float)sound->playTime * (1.0f / PLAY_TIME_TICKS_PER_SECOND); } }
// Play time of the current dialog in seconds; when the stream reports none and has no voice (not started yet, or a line
// without audio), the time measured by UpdateDialog instead. (inferred)
// Also known as: TDialog (upstream readable / Sly 1 name; TDialog__FP8SOUNDMGR).
extern "C" float GetDialogTime(SNDMGR *soundManager)
{
    if (!soundManager->dialogSound) return 0.0f;
    if (PlaySeconds(soundManager->dialogSound) == 0.0f) {
        STREAMSND *dialog = soundManager->dialogSound;
        int isStopped = 0;
        if (!dialog->voiceHandle) { isStopped = dialog->playState || (dialog->flags & SND_START); isStopped ^= 1; }
        if (isStopped) return soundManager->dialogElapsed;
    }
    return PlaySeconds(soundManager->dialogSound);
}
} // namespace M_func_001FE888

namespace M_func_001FE990 {
// Index of an id among the ambience stream's groups, or -1 (used to attach ambient emitters to groups). (inferred)
// Also known as: IvoiceFromTarget (upstream readable / Sly 1 name; IvoiceFromTarget__FP8SOUNDMGRi).
extern "C" int FindStreamGroupIndex(SNDMGR *soundManager, int id)
{
    int i;
    for (i = 0; i < soundManager->numAmbienceGroups; i++)
        if (soundManager->ambienceGroupIds[i] == id) return i;
    return -1;
}
} // namespace M_func_001FE990

// Sets the 989snd output mode (mono / stereo option) if it changed.
// Also known as: SetSoundPlaybackMode (upstream readable / Sly 1 name; SetSoundPlaybackMode__FP8SOUNDMGRi).
extern "C" void SetSndmgrPlaybackMode(SNDMGR *soundManager, int nMode)
{
    if (soundManager->playbackMode != nMode) {
        soundManager->playbackMode = nMode;
        snd_SetPlaybackMode(nMode);
    }
}

namespace M_SetSndmgrMasterVolume {
struct S;
extern struct S g_lmZeroOne __asm__("g_lmZeroOne");   /* the volume limits (0..1) */
float ClampToLimits__FP2LMf(struct S *, float) __asm__("ClampToLimits__FP2LMf");
void snd_SetMasterVolume(int, int) __asm__("snd_SetMasterVolume");
#define SND_MASTER_VOLUME_ALL 0x10   /* 989snd: the overall master (0..3 are the per-channel masters) (inferred) */
// Sets the master volume (clamped) on the 989snd library.
// Also known as: SetSoundMasterVolume (upstream readable / Sly 1 name; SetSoundMasterVolume__FP8SOUNDMGRf).
extern "C" void SetSndmgrMasterVolume(SNDMGR *soundManager, float volume)
{
    float clamped = ClampToLimits__FP2LMf(&g_lmZeroOne, volume);
    int level = (int)(clamped * 1024.0f);   /* 989snd volumes are 0..1024 */
    if ((int)(soundManager->masterVolume * 1024.0f) != level) {
        soundManager->masterVolume = clamped;
        snd_SetMasterVolume(SND_MASTER_VOLUME_ALL, level);
    }
}
} // namespace M_SetSndmgrMasterVolume

namespace M_SetSndmgrChannelVolume {
typedef void (STREAMSND::*PMF)();

extern LM g_lmZeroOne __asm__("g_lmZeroOne");
extern PMF D_002A8298 __asm__("D_002A8298"),D_002A82A0 __asm__("D_002A82A0");
extern float ClampToLimits__FP2LMf(LM *,float) __asm__("ClampToLimits__FP2LMf");
extern void *func_002333B4(void *,int,unsigned int) __asm__("memset");
extern void snd_SetMasterVolume(int,int) __asm__("snd_SetMasterVolume");
// Load *pp into $4.
static inline STREAMSND *ReadReceiver(STREAMSND **pp)
{
    register STREAMSND *receiver __asm__("$4");
    __asm__("lw %0,%1":"=r"(receiver):"m"(*pp));
    return receiver;
}
// Sets one category's weight on a volume channel, recomputes the channel's output volume, sends it and updates the live
// streams.
// Also known as: SetSoundChannelVolume (upstream readable / Sly 1 name; SetSoundChannelVolume__FP8SOUNDMGRiif).
extern "C" void SetSndmgrChannelVolume(SNDMGR *soundManager,int channel,int category,float volume)
{
    if((unsigned int)category>=8)return;
    if(channel<0)return;
    if(channel>=4)return;
    volume=ClampToLimits__FP2LMf(&g_lmZeroOne,volume);
    float (*volumes)[4]=soundManager->volumes;
    if(soundManager->volumes[category][channel]==volume)return;
    if(category==VOLUME_CATEGORY_PAUSE_FADE){
        PMF action;
        func_002333B4(&action,0,8);
        /* this category at 0 pauses the channel's sounds; leaving 0 resumes them */
        if(volume==0.0f)action=D_002A8298;   /* &SND::Pause (PauseSnd) */
        else if(volumes[2][channel]==0.0f)action=D_002A82A0;   /* &SND::Resume (ResumeSnd) */
        if(action!=0){
            switch(channel){
            case VOLUME_CHANNEL_AMBIENCE:if(soundManager->ambienceSound)(soundManager->ambienceSound->*action)();break;
            case VOLUME_CHANNEL_MUSIC:for(int i=0;i<soundManager->musicStackCount;i++){STREAMSND **slot=&soundManager->musicStack[i].p;STREAMSND *music=ReadReceiver(slot);if(music){union {STREAMSND *p;unsigned w;} r;r.p=music;(((STREAMSND *)r.w)->*action)();}}break;
            case VOLUME_CHANNEL_DIALOG:if(soundManager->dialogSound)(soundManager->dialogSound->*action)();break;
            }
        }
    }
    soundManager->volumes[category][channel]=volume;
    volume=1.0f;   /* from here: the channel's volume, the product of its categories */
    for(int i=0;i<8;i++)volume*=soundManager->volumes[i][channel];
    int level=(int)(volume*1024.0f);
    if((int)(soundManager->channelVolumes[channel]*1024.0f)!=level){
        soundManager->channelVolumes[channel]=volume;
        snd_SetMasterVolume(channel,level);
    }
}

typedef char layout_pointer[(sizeof(void*)==4)?1:-1];
typedef char layout_int[(sizeof(int)==4)?1:-1];
typedef char layout_float[(sizeof(float)==4)?1:-1];
typedef char layout_member_pointer[(sizeof(PMF)==8)?1:-1];

} // namespace M_SetSndmgrChannelVolume

namespace M_ResetSndmgrVolumes {
void SetSndmgrMasterVolume(void *, float) __asm__("SetSndmgrMasterVolume");
void SetSndmgrChannelVolume(void *, int, int, float) __asm__("SetSndmgrChannelVolume");
// Resets the master volume and all channel weights to 1 (category 1 excepted).
// Also known as: ResetSoundVolumes (upstream readable / Sly 1 name; ResetSoundVolumes__FPv).
extern "C" void ResetSndmgrVolumes(void *soundManager)
{
    int category, channel;
    SetSndmgrMasterVolume(soundManager, 1.0f);
    for (category = 0; category < 8; category++) {
        if (category == 1) continue;   /* the options menu's volumes (menu.cpp) are kept */
        for (channel = 0; channel < 4; channel++) SetSndmgrChannelVolume(soundManager, channel, category, 1.0f);
    }
}
} // namespace M_ResetSndmgrVolumes

// Adds a request for music layer 2 (counted; AI code raises it, inferred: the alert layer).
extern "C" void AddMusicLayer2Request(SNDMGR *soundManager)
{
    soundManager->musicLayer2Requests++;
}

// Removes a request for music layer 2. (inferred)
extern "C" void RemoveMusicLayer2Request(SNDMGR *soundManager)
{
    soundManager->musicLayer2Requests--;
}

// Adds a request for music layer 0 (counted). (inferred)
extern "C" void AddMusicLayer0Request(SNDMGR *soundManager)
{
    soundManager->musicLayer0Requests++;
}

// Removes a request for music layer 0. (inferred)
extern "C" void RemoveMusicLayer0Request(SNDMGR *soundManager)
{
    soundManager->musicLayer0Requests--;
}

// .rodata order (docs/guides/tu-merging.md "Phase 5: data order"): a named constant is emitted where it is
// defined, a function's literals and jump tables with the function, so each constant is defined where the ELF
// has it among the functions, under an asm label (no function sees a definition under the name it reads).
namespace RO_ORDER { extern const SMP r_D_002A8288 __asm__("D_002A8288") = {2.0f, 0.0f, 0.2f}; }   /* copied whole as three words */
namespace RO_ORDER {   /* pointers through asm-labelled declarations */
void p_PauseSnd() __asm__("PauseSnd");
extern const unsigned int r_D_002A8298[2] __asm__("D_002A8298") = {0xFFFF0000, (unsigned int)p_PauseSnd};
}
namespace RO_ORDER {   /* pointers through asm-labelled declarations */
void p_ResumeSnd() __asm__("ResumeSnd");
extern const unsigned int r_D_002A82A0[2] __asm__("D_002A82A0") = {0xFFFF0000, (unsigned int)p_ResumeSnd};
}
namespace M_func_001FEED0 {
/* 989snd play parameters (as in PlayBanksnd) */
struct M { int bank; int pad4; int volume; int pan; short soundIndex; short pad12; int pad14[3]; short pitch; short pad22; int pad24; int mask; int pad2C; };
#define SFX_VARIATION 0x28
int RandomIntInRange__Fii(int, int) __asm__("RandomIntInRange__Fii");
void *func_002333B4(void *, int, int) __asm__("memset");
short GetSoundIndex(SNDMGR *, void *) __asm__("GetSoundIndex");
void snd_PlaySoundEx(struct M *) __asm__("snd_PlaySoundEx");
// After one of the hero's own effects (PlayBanksnd), unless a music layer other than 0 is selected: a random step of the
// variation value in -12..12 (mostly +1, sometimes -5; b picks a row of a by the value, and the higher the value the
// more -5 entries the row has), then plays effect 0x28 with 127 x the value as its pitch modulation. (inferred)
// Also known as: StepSoundMood (upstream readable / Sly 1 name; StepSoundMood__FP8SOUNDMGR).
extern "C" void StepRandomSfxVariation(SNDMGR *soundManager)
{
    int r;
    if (soundManager->selectedMusicLayer) return;
    {
    int a[7][6] = {
        { -5, -5, -5, -5, -5, -5 },
        { -5, -5, -5, -5, -5, 1 },
        { -5, -5, -5, -5, 1, 1 },
        { -5, -5, -5, 1, 1, 1 },
        { -5, -5, 1, 1, 1, 1 },
        { -5, 1, 1, 1, 1, 1 },
        { 1, 1, 1, 1, 1, 1 },
    };
    int b[25] = { 6, 6, 6, 6, 6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 4, 3, 2, 1, 0 };
    struct M params;
    r = RandomIntInRange__Fii(0, 5);
    soundManager->sfxVariationStep += a[b[soundManager->sfxVariationStep + 12]][r];
    func_002333B4(&params, 0, sizeof(params));
    params.mask = 0x13;   /* bank, sound, pitch */
    params.bank = soundManager->bank;
    params.soundIndex = GetSoundIndex(soundManager, FindSoundForSfxId(soundManager, SFX_VARIATION));
    params.pitch = soundManager->sfxVariationStep * 127;
    snd_PlaySoundEx(&params);
    }
}
} // namespace M_func_001FEED0

namespace M_func_001FF108 {
extern SNDMGR D_003DD040 __asm__("D_003DD040");
int FindStreamGroupIndex(SNDMGR *, int) __asm__("FindStreamGroupIndex");
void SetStreamsndGroupVolume(void *, int, float) __asm__("SetStreamsndGroupVolume");
void SetStreamGroupPan(void *, int, int) __asm__("SetStreamGroupPan");
// Splice command: sets the volume and pan of the ambience sub-group selected by id. (inferred)
extern "C" void SetAmbienceGroupMix(int unused, int groupId, int pan, float volume)
{
    int group = FindStreamGroupIndex(&D_003DD040, groupId);
    void *ambience = D_003DD040.ambienceSound;
    SetStreamsndGroupVolume(ambience, group, volume);
    SetStreamGroupPan(ambience, group, pan);
}
} // namespace M_func_001FF108


namespace M_func_00259B48 {
typedef struct { int pad[3]; void *vptr; } O;   /* a SND (BASIC: AddRef/Release) */
// Reference-counted pointer assignment: add a reference to o, release the old target, store o.
extern "C" { inline void AssignSoundRef(O **pp, O *o)
{
    if (o) ((BASIC *)o)->AddRef();
    if (*pp) ((BASIC *)*pp)->Release();
    *pp = o;
} }
} // namespace M_func_00259B48

namespace M_func_00259BB0 {
typedef struct { int pad[3]; void *vptr; } O;   /* a SND (BASIC: AddRef/Release) */
// Initialise a counted pointer to o, adding a reference.
extern "C" { inline void InitSoundRef(O **pp, O *o) { if (o) ((BASIC *)o)->AddRef(); *pp = o; } }
} // namespace M_func_00259BB0

// ---- Data owned by this TU, in address order (docs/guides/tu-merging.md "Data"). Defined after the functions
// so no function sees a definition (keeps the code as matched); the namespaced declarations above refer to these.
extern const LM D_002A83B8 = {500.0f, 3000.0f};
extern const LM D_002A83C0 = {500.0f, 1000.0f};
int D_002C3C40[6] = {1548, 1549, 1550, -1, 1551, 1552};

#include "sly2/port.h"
#if !defined(SLY2_EE_LAYOUT)
/* Native adapters for SLY2_SRET_LABEL (sly2/port.h; docs/guides/portability.md "Struct results and arm64"). The EE
   passes a struct result's address as a hidden first argument, so the declarations elsewhere that spell this file's
   functions with the other form (a by-value result for an explicit out-pointer, or the reverse) call the same code
   there; natively they bind to these entries, which have their spelling. Plain records with the definitions' sizes. */
namespace SRET_sound {
extern "C" SNDREF *FindMusicInto(SNDREF *out, SNDMGR *soundManager, int id)
{ SNDREFC r = M_func_001FE478::FindMusic(soundManager, id); out->p = r.p; r.p = 0; return out; }   /* moves into *out */
} // namespace SRET_sound
#endif
