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
// +0x200, +0x208[8] and +0x228 (slot 2 of each object's vtable), then runs ~BASIC (DestroyBasic, in-charge flag 2).
// Spelled as a base with that destructor and the pointers as members with an inline release, g++ generates the
// 123-instruction construct-and-destruct StaticInit_sound and the _GLOBAL_$D$ stub func_001FF398.
// The hand-written per-function files keep their views in namespaces named after each function (as jt.cpp does).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_sound
// SLY2_ALIAS: _GLOBAL_$I$*=func_001FF378
// SLY2_ALIAS: _GLOBAL_$D$*=func_001FF398

#include "sly2/basic.h"
#include "sly2/slotheap.h"
#include "sly2/cm.h"
#include "sly2/sndmgr.h"
#include "sly2/vu0.h"

/* g_sndmgr's static construction and destruction. */
struct VTESND { short d; short pad; void (*pfn)(void *); };
struct SNDOBJ { char pad0[0xC]; VTESND *vtable; };
/* A counted pointer to a sound object: released through its vtable slot 2. */
struct SNDREF
{
    SNDOBJ *p;
    ~SNDREF() { if (p) p->vtable[2].pfn((char *)p + p->vtable[2].d); }
};
struct SNDMGRG;
extern "C" {
void SNDMGR_ctor(SNDMGRG *soundManager);
void DestroyBasic(void *pv, int grfDelete);   /* ~BASIC */
}
struct SNDMGRBASE
{
    char ab[0x4C];
    DL dlSndDirty;          /* +0x4C [S] draft name: the sounds RemoveSndFromDirtyList takes out (was the address name D_003DD08C) */
    char ab58[0x200 - 0x58];
    ~SNDMGRBASE() { DestroyBasic(this, 2); }
};
struct SNDMGRG : public SNDMGRBASE
{
    SNDREF dialogSound;      /* +0x200 */
    int musicStackCount;     /* +0x204 */
    SNDREF musicStack[8];    /* +0x208 */
    SNDREF ambienceSound;    /* +0x228 */
    SNDMGRG() { SNDMGR_ctor(this); }
    ~SNDMGRG() {}
};
SNDMGRG D_003DD040;

// 0x1FF188 StaticInit_sound (g_sndmgr, then the six slot pools; g_sndmgr's destruction) and its stubs.
SLY2_INSTANTIATE_SLOT_POOLS()

/* A sound as the m2c bodies read it: vtable +0xC, pan +0x14, pitch +0x18, voice handle +0x40, refcount +0x4C,
   fade time +0x48; a bank sound's sfx id at +0x60. */
struct SNDV
{
    char pad0[0xC];
    VTESND *vtable;            /* +0x0C */
    char pad10[4];
    int panAngle;               /* +0x14 */
    float pitchRatio;           /* +0x18 */
    char pad1C[0x40 - 0x1C];
    int voiceHandle;             /* +0x40 */
    char pad44[0x48 - 0x44];
    float fadeTime;           /* +0x48 */
    ushort refCount;            /* +0x4C */
    char pad4E[0x60 - 0x4E];
    int sfxId;              /* +0x60 (BANKSND) */
};
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
/* volume channels (GetStreamVolumeChannel, SetSndmgrChannelVolume) and the volume category that pauses a channel's
   sounds at 0 (SetSndmgrChannelVolume; UpdateSndmgr fades it while game time is stopped). Macros, not enums: an enum
   constant passed to a function declared per namespace can make the call ambiguous for g++ 2.95. */
#define VOLUME_CHANNEL_AMBIENCE 0
#define VOLUME_CHANNEL_MUSIC 1
#define VOLUME_CHANNEL_DIALOG 2
#define VOLUME_CHANNEL_EFFECTS 3
#define VOLUME_CATEGORY_PAUSE_FADE 2
#define PLAY_TIME_TICKS_PER_SECOND 294912000.0f   /* a stream's play time (+0x68) counts these */
#define CD_STATE_IDLE 3   /* cd.cpp's disc-poll state D_002BD4C0 (1 read, 2 wait, 3 idle) */
/* A stream record (FindDialogStream) with a float parameter at +0x50. */
struct STREAMREC { char pad0[0x50]; float duration; };

extern "C" {
extern void *D_002D9C70[] __asm__("_vt$7BANKSND");  /* BANKSND vtable */
void SetSndFlags(SNDV *sound, int grfMask, int grf);
SNDV *SND_ctor(SNDV *sound);
SNDV *BANKSND_ctor(SNDV *sound, int sfxId);
void *AllocBanksnd(int cb);
int *FindSoundForSfxId(SNDMGR *soundManager, int sfxId);
void snd_PauseSound(int voiceHandle);
void snd_ContinueSound(int voiceHandle);
void *AllocZeroedFromSlotheap(SLOTHEAP *pslotheap);
void FreeSlotheapSlot(SLOTHEAP *pslotheap, void *pv);
void func_00233088(int nRpc, int nCmd, int n, int n3, int n4, int n5, int n6);   /* the IOP sound RPC */
STREAMREC *FindDialogStream(int id);
void snd_SetPlaybackMode(int nMode);
}
int IsEntryInDl(DL *pdl, void *pv);
void AppendDlEntry(DL *pdl, void *pv);

namespace M_NewSfx { extern "C" {
typedef struct MSG { int cb; int cbAlign; } MSG;   /* SWALLOC */
void *AllocClearedFromWorldHeap(MSG *pmsg);   /* PvAllocSwClear */
struct E { int sfxId; float rangeNear; float rangeFar; float volume; int unknown10; };   /* SFX */
// Allocates a sound-effect descriptor (as objects load one from the level) on the world heap: no effect, full volume,
// audible from 300 (full) to 3000 units (silent).
struct E *NewSfx(void)
{
    MSG msg;
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
} } // namespace M_NewSfx

namespace M_SND_ctor { extern "C" {
extern char D_002D9D00[] __asm__("_vt$6CID151");
struct S { int a; int b; int c; void *vt; float volume; char pad[0x1C]; float rangeNear; float rangeFar; char pad2[0xC]; float effectiveVolume; };
// Base sound constructor: BASIC header cleared, SND vtable, volume 1, audible from 300 to 3000 units.
struct S *SND_ctor(struct S *sound)
{
    sound->vt = D_002D9D00;
    sound->a = 0;
    sound->b = 0;
    sound->volume = 1.0f;
    sound->effectiveVolume = 1.0f;
    sound->rangeNear = 300.0f;
    sound->rangeFar = 3000.0f;
    return sound;
}
} } // namespace M_SND_ctor

namespace M_PauseSnd { extern "C" {
void SetSndFlags(CID151 *, int, int);
// Pauses a sound (counted: nested pauses need as many resumes); the first pause requests "pause" on the next flush.
void PauseSnd(CID151 *sound) { if (++sound->pauseCount == 1) SetSndFlags(sound, SND_PAUSE | SND_CONTINUE, SND_PAUSE); }
} } // namespace M_PauseSnd

namespace M_ResumeSnd { extern "C" {
void SetSndFlags(CID151 *, int, int);
// Undoes one PauseSnd; the last one requests "continue" on the next flush.
void ResumeSnd(CID151 *sound) { if (--sound->pauseCount == 0) SetSndFlags(sound, SND_PAUSE | SND_CONTINUE, SND_CONTINUE); }
} } // namespace M_ResumeSnd

// Requests that a sound stop, fading out over dtFade seconds (applied by the next UpdateSnd).
extern "C" void StopSnd(SNDV *sound, float fadeTime)
{
    sound->fadeTime = fadeTime;
    SetSndFlags(sound, SND_ALL, SND_STOP);
}

namespace M_StartSnd { extern "C" {
typedef int qword __attribute__((mode(TI)));
extern qword D_002A2DD0;
int AreFloatsNear__Ffff(float, float, float);
void SetSndFlags(CID151 *, int, int);
static inline int AreVectorsNear(qword a, qword b)
{
    return AreFloatsNear__Ffff(QwordX(a), QwordX(b), 0.0001f) && AreFloatsNear__Ffff(QwordY(a), QwordY(b), 0.0001f) && AreFloatsNear__Ffff(QwordZ(a), QwordZ(b), 0.0001f);
}
} } // namespace M_StartSnd
// Starts a sound (slot 7) unless it is already playing or starting: requests start and marks every parameter dirty
// (plus pause if paused, plus positional if it has a source).
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

namespace M_func_001FB1F8 { extern "C" {
struct S;
extern struct S D_002BBB60;
float ClampToLimits__FP2LMf(struct S *, float);
void SetSndFlags(CID151 *, int, int);
// Sets a sound's volume (clamped to 0..1); marks it dirty only if it changed by at least 1/1024.
void SetSndVolume(CID151 *sound, float volume)
{
    float clamped = ClampToLimits__FP2LMf(&D_002BBB60, volume);
    if ((int)(clamped * 1024.0f) != (int)(sound->volume * 1024.0f)) {
        sound->volume = clamped;
        SetSndFlags(sound, SND_DIRTY_VOLUME, SND_DIRTY_VOLUME);
    }
}
} } // namespace M_func_001FB1F8

// Sets a sound's pan angle in degrees (normalized to 0..359) and marks it dirty.
extern "C" void SetSndPan(SNDV *sound, int deg)
{
    int panAngle = deg % 360;
    panAngle = (panAngle > -1) ? panAngle : panAngle + 360;
    if (panAngle != sound->panAngle) {
        sound->panAngle = panAngle;
        SetSndFlags(sound, SND_DIRTY_PAN, SND_DIRTY_PAN);
    }
}

// Sets a sound's pitch and marks it dirty.
extern "C" void SetSndPitch(SNDV *sound, int n1, int n2, float pitchRatio)
{
    if (pitchRatio != sound->pitchRatio) {
        sound->pitchRatio = pitchRatio;
        SetSndFlags(sound, SND_DIRTY_PITCH, SND_DIRTY_PITCH);
    }
}

namespace M_SetSndSourceLo { extern "C" {
typedef int qword __attribute__((mode(TI)));
extern qword D_002A2DD0;
int AreFloatsNear__Ffff(float, float, float);
void SetSndFlags(CID151 *, unsigned short, unsigned short);
static inline int AreVectorsNear(qword a, qword b)
{
    return AreFloatsNear__Ffff(QwordX(a), QwordX(b), 0.0001f) && AreFloatsNear__Ffff(QwordY(a), QwordY(b), 0.0001f) && AreFloatsNear__Ffff(QwordZ(a), QwordZ(b), 0.0001f);
}
// Makes a sound positional, following an object.
void SetSndSourceLo(CID151 *sound, LO *source)
{
    sound->source = source;
    if (source) SetSndFlags(sound, SND_POSITIONAL, SND_POSITIONAL);
    else if (AreVectorsNear(D_002A2DD0, sound->position)) SetSndFlags(sound, SND_POSITIONAL, 0);
}
} } // namespace M_SetSndSourceLo

namespace M_SetSndSourcePos { extern "C" {
typedef int qword __attribute__((mode(TI)));
extern qword D_002A2DD0;
int AreFloatsNear__Ffff(float, float, float);
void SetSndFlags(CID151 *, int, int);
static inline int AreVectorsNear(qword a, qword b)
{
    return AreFloatsNear__Ffff(QwordX(a), QwordX(b), 0.0001f) && AreFloatsNear__Ffff(QwordY(a), QwordY(b), 0.0001f) && AreFloatsNear__Ffff(QwordZ(a), QwordZ(b), 0.0001f);
}
// Makes a sound positional at a fixed world position (a zero vector means not positional).
void SetSndSourcePos(CID151 *sound, qword position)
{
    sound->position = position;
    if (!AreVectorsNear(D_002A2DD0, position)) SetSndFlags(sound, SND_POSITIONAL, SND_POSITIONAL);
    else if (!sound->source) SetSndFlags(sound, SND_POSITIONAL, 0);
}
} } // namespace M_SetSndSourcePos

namespace M_SetSndRange { extern "C" {
// Sets the distance at which a positional sound starts to fade (full) and the distance where it is silent (stop);
// stop is forced to be at least full + 1.
void SetSndRange(CID151 *sound, float rangeNear, float rangeFar) { if (rangeFar <= rangeNear) { rangeFar = rangeNear + 1.0f; sound->rangeFar = rangeFar; } else sound->rangeFar = rangeFar; sound->rangeNear = rangeNear; }
} } // namespace M_SetSndRange

/* SetSndDoppler: empty */
// Empty in the release build; called with a fade/doppler argument (inferred).
extern "C" void SetSndDoppler()
{
}

namespace M_UpdateSndPositional { extern "C" {
typedef int qword __attribute__((mode(TI)));
typedef unsigned long ulong;
struct VTE { short d; short pad; void (*pfn)(void *, int); };
struct O { char pad[0x28]; ulong fl; };
struct P { char pad[0xC]; struct VTE *vtable; float volume; char pad1[0x1C - 0x14]; struct O *source; qword position; char range[0x14]; float effectiveVolume; };
void StopSnd(struct P *, float);
qword ConvertPosBetweenLos(struct O *, int, qword);
void CalculateSndVolPan(struct P *, qword, void *, float *, int *);
void SetSndFlags(struct P *, int, int);
void SetSndPan(struct P *, int);
// Positional update (slot 16): volume from the distance to the listener and pan from its direction; switches the
// sound's culled state when it becomes audible or silent.
void UpdateSndPositional(struct P *sound)
{
    float fade;
    int panAngle;
    float volume;
    if (sound->source && (((sound->source->fl >> 60) & 1) == 0)) {
        StopSnd(sound, 0.0f);
        return;
    }
    CalculateSndVolPan(sound, ConvertPosBetweenLos(sound->source, 0, sound->position), sound->range, &fade, &panAngle);
    volume = sound->volume * fade;
    if ((int)(volume * 1024.0f) != (int)(sound->effectiveVolume * 1024.0f)) {
        if (sound->effectiveVolume == 0.0f) sound->vtable[15].pfn((char *)sound + sound->vtable[15].d, 0);
        else if (volume == 0.0f) sound->vtable[15].pfn((char *)sound + sound->vtable[15].d, 1);
        sound->effectiveVolume = volume;
        SetSndFlags(sound, SND_DIRTY_VOLUME, SND_DIRTY_VOLUME);
    }
    SetSndPan(sound, panAngle);
}
} } // namespace M_UpdateSndPositional

namespace M_SetSndFlags { extern "C" {
void AddSndToDirtyList(CID151 *);
void RemoveSndFromDirtyList(CID151 *);
// Sets the masked bits of a sound's request flags; a sound with pending requests is put on the manager's dirty list,
// one without is taken off it.
void SetSndFlags(CID151 *sound, unsigned short mask, unsigned short value)
{
    sound->flags = (sound->flags & ~mask) | (value & mask);
    if (sound->flags) AddSndToDirtyList(sound); else RemoveSndFromDirtyList(sound);
}
} } // namespace M_SetSndFlags

// Puts a sound on the manager's dirty list (taking a reference) if it is not already there.
extern "C" void AddSndToDirtyList(SNDV *sound)
{
    if (IsEntryInDl(&G_SNDMGR.dirtySounds, sound) == 0) {
        VTESND *vtable = sound->vtable;
        vtable[1].pfn((char *)sound + vtable[1].d);   /* Incref */
        AppendDlEntry(&G_SNDMGR.dirtySounds, sound);
    }
}

namespace M_RemoveSndFromDirtyList { extern "C" {
struct VT { int pad[4]; short d; short p; void (*pfn)(void *); };
struct X { char pad[0xC]; struct VT *vtable; };
struct DL;
int IsEntryInDl__FP2DLPv(struct DL *, void *);
void RemoveDlEntry__FP2DLPv(struct DL *, void *);
// Takes a sound off the dirty list and drops the list's reference.
void RemoveSndFromDirtyList(struct X *sound)
{
    if (IsEntryInDl__FP2DLPv((struct DL *)&D_003DD040.dlSndDirty, sound)) {
        RemoveDlEntry__FP2DLPv((struct DL *)&D_003DD040.dlSndDirty, sound);
        sound->vtable->pfn((char *)sound + sound->vtable->d);
    }
}
} } // namespace M_RemoveSndFromDirtyList

namespace M_CalculateSndVolPan { extern "C" {
typedef int qword __attribute__((mode(TI)));
typedef union {qword q;float f[4];int i[4];} V;
struct RANGE {float low,high;};
struct M3 {qword r[3];};
struct CAMERA {char pad[0x90];qword position;};
extern char D_003DD490[],D_002BBB60[];
extern struct CAMERA D_002DDF40;
void *GetCurrentPo(void*);
qword GetLoWorldPos(void*);
qword ConvertMatVec(const struct M3*,const struct M3*,qword);
float ClampToLimits__FP2LMf(void*,float);
float atan2f(float,float);
static inline qword Sub(qword a,qword b){register qword r __asm__("vf3");__asm__("vsub.xyzw %0,%1,%2":"=&j"(r):"j"(a),"j"(b));return r;}
static inline float Length(qword a,V*save){qword d,one;float g;
__asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=&j"(d),"=&j"(one):"j"(a));
__asm__("vsqrt $Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q":"=j"(d):"j"(d));
save->q=a;
__asm__("qmfc2.ni %0,%1":"=r"(g):"j"(d));return g;}
static inline float Y(qword q){int bits;float f;__asm__("prot3w %0,%1":"=r"(bits):"r"(q));__asm__("mtc1 %1,%0":"=f"(f):"r"(bits));return f;}
static inline qword Bits(qword v){qword q;__asm__("qmfc2.ni %0,%1":"=r"(q):"j"(v));return q;}
static inline void Rounded(int*out,float f){float bits;__asm__("cvt.w.s %0,%1":"=&f"(bits):"f"(f));__asm__("swc1 %1,%0":"=m"(*out):"f"(bits));}
// Volume and pan of a point as heard by the listener (the player, or the camera): volume falls linearly from 1 at the
// full distance to 0 at the stop distance; pan is the angle to the point in degrees.
void CalculateSndVolPan(void *unused,qword point,const struct RANGE*limits,float*outValue,int*outAngle)
{
 V source,delta;
 void *player;
 register qword reference __asm__("vf1");
 float high,low,distance,value;
 register qword difference __asm__("vf3");
 source.q=point;
 player=GetCurrentPo(D_003DD490);
 if(player)reference=GetLoWorldPos(player);
 else reference=D_002DDF40.position;
 high=limits->high;low=limits->low;
 {register qword input __asm__("vf2")=source.q;
  distance=Length(Sub(input,reference),&delta);}
 value=ClampToLimits__FP2LMf(D_002BBB60,(high-distance)/(high-low));
 difference=delta.q;
 if(outValue)*outValue=value;
 if(outAngle){
   if(value>0.0f){
     qword direction;
     float angle;
     direction=ConvertMatVec(0,&(*(struct M3 *)&g_cm.mat),Bits(difference));
     angle=atan2f(-Y(direction),QwordX(direction));
     if(limits->high<limits->low)angle+=3.1415927f;
     Rounded(outAngle,angle*57.295776f);return;
   }else *outAngle=0;
 }
}
} } // namespace M_CalculateSndVolPan

namespace M_UpdateSnd {
struct VTE { short d; short p; void (*pfn)(void *); };
struct P { char pad[0xC]; VTE *vtable; float volume; char pad2[0x40 - 0x14]; int voiceHandle; float effectiveVolume; char pad3[0x50 - 0x48]; unsigned short flags; unsigned short playState; };
extern "C" void SetSndFlags(P *, int, int);
#define VCALL(p, n) ((p)->vtable[n].pfn((char *)(p) + (p)->vtable[n].d))
#define ICALL(p, n) (((int (*)(void *))(p)->vtable[n].pfn)((char *)(p) + (p)->vtable[n].d))
/* virtual slots of SND (BANKSND / STREAMSND override them) */
#define SLOT_INCREF 1
#define SLOT_DECREF 2
#define SLOT_GET_KIND 8
#define SLOT_PLAY 9
#define SLOT_STOP 10
#define SLOT_PAUSE 11
#define SLOT_CONTINUE 12
#define SLOT_APPLY_PARAMS 13
#define SLOT_QUERY_STATUS 14
#define SLOT_UPDATE_POSITIONAL 16
/* holds a reference for the duration of the update */
struct Lock {
    P *sound;
    Lock(P *locked) { sound = locked; VCALL(sound, SLOT_INCREF); }
    ~Lock() { VCALL(sound, SLOT_DECREF); }
};
// Flushes a dirty sound to the sound hardware: positional update, then start, stop, status query, parameters, pause and
// continue by its request bits (each through its virtual slot).
extern "C" void UpdateSnd(P *sound)
{
    Lock lock(sound);
    unsigned short flags;
    if (sound->flags & SND_POSITIONAL) VCALL(sound, SLOT_UPDATE_POSITIONAL);
    else sound->effectiveVolume = sound->volume;
    flags = sound->flags;
    if (flags & SND_START) {
        VCALL(sound, SLOT_PLAY);
        if (ICALL(sound, SLOT_GET_KIND) == 1) SetSndFlags(sound, SND_QUERY_PLAYING, SND_QUERY_PLAYING);   /* one-shot: watch for its end */
    } else if (sound->voiceHandle) {
        if (flags & SND_STOP) {
            VCALL(sound, SLOT_STOP);
            sound->voiceHandle = 0;
            SetSndFlags(sound, SND_ALL & ~SND_POSITIONAL, 0);
        } else {
            if (flags & SND_QUERIES) VCALL(sound, SLOT_QUERY_STATUS);
            if (sound->playState) return;   /* still starting */
            if (sound->flags & SND_DIRTY_PARAMS) {
                VCALL(sound, SLOT_APPLY_PARAMS);
                SetSndFlags(sound, SND_DIRTY_PARAMS, 0);
            }
            flags = sound->flags;
            if (flags & SND_PAUSE) {
                VCALL(sound, SLOT_PAUSE);
                SetSndFlags(sound, SND_PAUSE, 0);
            } else if (flags & SND_CONTINUE) {
                VCALL(sound, SLOT_CONTINUE);
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

namespace M_func_001FBAF8 { extern "C" {
// Async callback when a looping bank sound started: stores its voice handle, clears "starting", drops the callback's
// reference. (inferred)
void OnBankSoundStarted(int voiceHandle, long context) { CID151 *sound = (CID151 *)(int)context; sound->voiceHandle = voiceHandle; sound->playState = 0; sound->Release(); }
} } // namespace M_func_001FBAF8

namespace M_func_001FBB30 { extern "C" {
struct VTE { short d; short pad; void (*pfn)(void *); };
struct A { char pad[0xC]; struct VTE *vt; };
void StopSnd(struct A *sound, float fadeTime);
void SetSndFlags(struct A *sound, int mask, int value);
// Async "is it still playing" callback: a finished sound is stopped, a playing one gets its next status query armed;
// drops the callback's reference. (inferred)
void OnBankSoundStatus(int isPlaying, long context)
{
    struct A *sound = (struct A *)context;
    if (!isPlaying)
        StopSnd(sound, 0.0f);
    else
        SetSndFlags(sound, SND_QUERY_PLAYING, SND_QUERY_PLAYING);
    sound->vt[2].pfn((char *)sound + sound->vt[2].d);
}
} } // namespace M_func_001FBB30

// Creates a bank sound for a sound-effect id from the manager's pool; it starts at the next flush.
extern "C" void CreateSfxSound(int sfxId)
{
    BANKSND_ctor((SNDV *)AllocBanksnd(0x70), sfxId);
}

// Bank-sound constructor: base sound constructor, then the start request.
extern "C" SNDV *BANKSND_ctor(SNDV *sound, int sfxId)
{
    SND_ctor(sound);
    sound->sfxId = sfxId;
    sound->vtable = (VTESND *)D_002D9C70;
    SetSndFlags(sound, SND_START, SND_START);
    return sound;
}

// Kind of a bank sound (slot 8): 2 for looping effects, from the level's sound table, else -1.
extern "C" int GetBanksndKind(SNDV *sound)
{
    int *pn = FindSoundForSfxId(&G_SNDMGR, sound->sfxId);
    int n = -1;
    if (pn != 0) {
        n = *pn;
    }
    return n;
}

namespace M_PlayBanksnd { extern "C" {
struct VTE { short d; short p; void (*pfn)(void *); };
struct P { char pad[0xC]; struct VTE *vtable; char pad1[0x14 - 0x10]; int panAngle; float pitchRatio; char pad2[0x44 - 0x1C]; float effectiveVolume; char pad3[0x50 - 0x48]; unsigned short flags; unsigned short playState; char pad4[0x60 - 0x54]; int sfxId; };
struct G { char pad[0xE8]; int bank; };   /* SNDMGR */
struct S { int kind; };   /* a sound's entry in SNDMGR soundKinds */
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
extern struct G D_003DD040;
extern char D_003DD490[];
extern int D_002DD5BC, D_002F7900, D_002E1E40;   /* Bentley, Murray, Sly */
struct S *FindSoundForSfxId(struct G *, int);
short GetSoundIndex(struct G *, struct S *);
void SetSndFlags(struct P *, int, int);
void RemoveSndFromDirtyList(struct P *);
void *func_002333B4(void *, int, int) __asm__("memset");
void OnBankSoundStarted(void);
void func_00232320(struct M *, void (*)(void), struct P *);
void func_002322F0(struct M *);
int GetHeroPo(void *);
void StepRandomSfxVariation(struct G *);
// Starts a bank sound on the 989snd library (slot 9) with its bank, sound index, volume, pan and pitch: loopers start
// asynchronously and get a voice handle, one-shots are fire-and-forget; some effects get special handling.
void PlayBanksnd(struct P *sound)
{
    struct G *soundManager = &D_003DD040;
    struct S *entry = FindSoundForSfxId(soundManager, sound->sfxId);
    struct M params;
    int owner;
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
    if (entry->kind == SOUND_KIND_LOOPING) {
        sound->playState = 1;
        sound->vtable[1].pfn((char *)sound + sound->vtable[1].d);
        func_00232320(&params, OnBankSoundStarted, sound);
        SetSndFlags(sound, SND_START | SND_PAUSE | SND_CONTINUE | SND_STOP | SND_DIRTY_SOUND_PARAMS, 0);
        return;
    }
    owner = 0;
    func_002322F0(&params);
    SetSndFlags(sound, SND_ALL, 0);
    switch (sound->sfxId) {
    case SFX_SLY: owner = D_002E1E40; break;
    case SFX_BENTLEY: owner = D_002DD5BC; break;
    case SFX_MURRAY: owner = D_002F7900; break;
    }
    if (owner == GetHeroPo(D_003DD490)) StepRandomSfxVariation(&D_003DD040);
}
} } // namespace M_PlayBanksnd

namespace M_StopBanksnd { extern "C" {
void snd_AutoVol(int voiceHandle, int a, int b, int c);
void snd_StopSound(int voiceHandle);
// Stops a bank sound (slot 10): a fade-out when a fade time was requested, else immediately.
void StopBanksnd(CID151 *sound)
{
    if (sound->fadeTime > 0.0f)
        snd_AutoVol(sound->voiceHandle, -4,   /* fade to silence over fadeTime (240 ticks per second) */
                     (int)(sound->fadeTime * 240.0f), 2);
    else
        snd_StopSound(sound->voiceHandle);
}
} } // namespace M_StopBanksnd

// Pauses a bank sound's voice (slot 11).
extern "C" void PauseBanksnd(SNDV *sound)
{
    snd_PauseSound(sound->voiceHandle);
}

// Resumes a bank sound's voice (slot 12).
extern "C" void ContinueBanksnd(SNDV *sound)
{
    snd_ContinueSound(sound->voiceHandle);
}

namespace M_ApplyBanksndParams { extern "C" {
struct X { char pad[0x14]; int panAngle; float pitchRatio; char pad2[0x40 - 0x1C]; void *voiceHandle; float effectiveVolume; char pad3[0x50 - 0x48]; unsigned short flags; };
void func_00232500(void *, int, int, int, int, int);   /* 989snd: set voice volume / pan / pitch (inferred) */
// Sends a bank sound's changed volume, pan and pitch to its voice (slot 13).
void ApplyBanksndParams(struct X *sound)
{
    int flags = sound->flags;
    if (flags & SND_DIRTY_SOUND_PARAMS) {
        int changed = 0;
        int volume, pitch;
        if (flags & SND_DIRTY_VOLUME) { changed = 1; volume = (int)(sound->effectiveVolume * 1024.0f); }
        if (flags & SND_DIRTY_PAN) changed |= 4;
        if (flags & SND_DIRTY_PITCH) { changed |= 8; pitch = (int)(sound->pitchRatio * 12.0f); }
        func_00232500(sound->voiceHandle, changed, volume, sound->panAngle, pitch, 0);
    }
}
} } // namespace M_ApplyBanksndParams

namespace M_BANKSND_QueryStatus { extern "C" {
struct VT { int pad[2]; short d; short p; void (*pfn)(void *); };
struct X { char pad[0xC]; struct VT *vtable; char pad2[0x40 - 0x10]; void *voiceHandle; char pad3[0x50 - 0x44]; unsigned short flags; };
void OnBankSoundStatus(void);
void snd_SoundIsStillPlaying_CB(void *, void (*)(void), void *);
void SetSndFlags(struct X *, int, int);
// Asks the IOP whether a bank sound's voice is still playing (slot 14), with OnBankSoundStatus as callback.
void BANKSND_QueryStatus(struct X *sound)
{
    if (sound->flags & SND_QUERY_PLAYING) {
        sound->vtable->pfn((char *)sound + sound->vtable->d);
        snd_SoundIsStillPlaying_CB(sound->voiceHandle, OnBankSoundStatus, sound);
        SetSndFlags(sound, SND_QUERY_PLAYING, 0);
    }
}
} } // namespace M_BANKSND_QueryStatus

namespace M_func_001FC018 { extern "C" {
struct VTE { short d; short p; void *pfn; };
struct VT { struct VTE a[9]; };
struct P { char pad[0xC]; struct VT *vtable; };
void SetSndFlags(struct P *, int, int);
// Culling hook (slot 15): a sound that becomes audible again restarts; one that becomes silent is stopped (loopers keep
// their parameters so they can restart). (inferred)
void SetBankSoundCulled(struct P *sound, int isCulled)
{
    if (!isCulled) ((void (*)(void *))sound->vtable->a[7].pfn)((char *)sound + sound->vtable->a[7].d);
    else {
        /* request the stop, clearing the other requests; a looper keeps its positional flag so the positional update
           can restart it when it becomes audible again */
        int looperMask = SND_ALL & ~SND_POSITIONAL & ~SND_QUERIES;
        SetSndFlags(sound, ((int (*)(void *))sound->vtable->a[8].pfn)((char *)sound + sound->vtable->a[8].d) == SOUND_KIND_LOOPING ? looperMask : SND_ALL & ~SND_QUERIES, SND_STOP);
    }
}
} } // namespace M_func_001FC018

// Adds a reference to a bank sound (slot 1).
void BANKSND::AddRef()   /* key function: BANKSND's vtable comes out here */
{
    SNDV *sound = (SNDV *)this;
    sound->refCount++;
}

namespace M_DecrefBanksnd { extern "C" {
void DestroyBasic(BANKSND *, int);
void FreeBanksnd(BANKSND *);
// Drops a reference to a bank sound (slot 2); at zero it is destroyed and returned to its pool. (inferred)
void DecrefBanksnd(BANKSND *sound) { if (--sound->refCount == 0 && sound) { DestroyBasic(sound, 0); FreeBanksnd(sound); } }
} } // namespace M_DecrefBanksnd

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
struct A; struct B;
struct X {
    static void *operator new(unsigned int cb) __asm__("AllocStreamsnd");
    X(A *pa, B *pb) __asm__("STREAMSND_ctor");
    char pad[0x3C0];
};
// Creates a streamed sound (music, dialog, ambience) for a stream record and kind, or 0 if the pool is empty.
extern "C" X *PstreamsndNew(A *pa, B *pb)
{
    X *px;
    if (pa)
        px = new X(pa, pb);
    else
        px = 0;
    return px;
}
} // namespace M_PstreamsndNew

namespace M_STREAMSND_ctor { extern "C" {
/* STREAMSND (0x3C0): SND, then the stream record (+0x60), play time (+0x68), six sub-groups' volumes (+0x70), pans
   (+0x88), source counts (+0xA0) and sources (+0xB8, 8 each), the stream kind (+0x3B8) and the dirty sub-group mask
   (+0x3BC). */
struct P { char pad[0xC]; void *vtable; char pad2[0x60 - 0x10]; int streamRecord; char pad3[0x70 - 0x64]; float groupVolumes[6]; char pad4[0x3B8 - 0x88]; int streamKind; };
/* SNDMGR: paused flag (+0x58), the pause-fade category's per-channel weights (+0x15C, row 2 of the volume table) */
struct G { char pad[0x58]; int isPaused; char pad2[0x15C - 0x5C]; float pauseFadeVolumes[1]; };
extern struct G D_003DD040;
extern char D_002D9BE0[] __asm__("_vt$9STREAMSND");
void SND_ctor(struct P *);
void SetSndFlags(struct P *, int, int);
int GetStreamVolumeChannel(struct G *, int);
void PauseSnd(struct P *);
// Streamed-sound constructor: base constructor, STREAMSND vtable, the file record and kind, all six sub-group volumes at
// 1; requests start and marks it timed, and starts paused when its channel's pause-fade weight is 0 or
// the manager is paused.
struct P *STREAMSND_ctor(struct P *sound, int streamRecord, int streamKind)
{
    int i;
    struct G *soundManager;
    SND_ctor(sound);
    sound->streamRecord = streamRecord;
    sound->vtable = D_002D9BE0;
    sound->streamKind = streamKind;
    for (i = 0; i < 6; i++)
        sound->groupVolumes[i] = 1.0f;
    soundManager = &D_003DD040;
    SetSndFlags(sound, SND_START | SND_TIMED, SND_START | SND_TIMED);
    /* start paused when its channel is silent or the manager is paused */
    if (*(soundManager->pauseFadeVolumes + GetStreamVolumeChannel(soundManager, sound->streamRecord)) == 0.0f)
        PauseSnd(sound);
    if (soundManager->isPaused)
        PauseSnd(sound);
    return sound;
}
} } // namespace M_STREAMSND_ctor

namespace M_StartStreamsnd { extern "C" {
void StartSnd(STREAMSND *);
// Starts a streamed sound (slot 7): base start, then resets its play time and status queries.
void StartStreamsnd(STREAMSND *sound)
{
    /* only when not already started: no voice, not playing, flag 1 clear */
    int canStart = 0; if (!sound->voiceHandle) { canStart = sound->playState || (sound->flags & SND_START); canStart ^= 1; } if (canStart) {
        StartSnd(sound);
        sound->playTime = 0;
        sound->dirtyGroups = 0xF;
    }
}
} } // namespace M_StartStreamsnd

namespace M_SetStreamsndGroupVolume { extern "C" {
struct S;
extern struct S D_002BBB60;   /* the volume limits (0..1) */
float ClampToLimits__FP2LMf(struct S *, float);
void SetSndFlags(STREAMSND *, int, int);
// Sets the volume of one of a stream's six sub-groups (channels of a multi-layer stream) and marks it dirty.
void SetStreamsndGroupVolume(STREAMSND *sound, unsigned int group, float volume)
{
    if (group < 6) {
        float clamped = ClampToLimits__FP2LMf(&D_002BBB60, volume);
        /* compared at the IOP's 1/1024 resolution */
        if ((int)(clamped * 1024.0f) != (int)(sound->groupVolumes[group] * 1024.0f)) {
            sound->dirtyGroups |= 1 << group;
            sound->groupVolumes[group] = clamped;
            SetSndFlags(sound, SND_DIRTY_GROUP_VOLUME, SND_DIRTY_GROUP_VOLUME);
        }
    }
}
} } // namespace M_SetStreamsndGroupVolume

namespace M_func_001FC380 { extern "C" {
void SetSndFlags(STREAMSND *, int, int);
static inline int Norm(int deg) { int n = deg % 360; return n > -1 ? n : n + 360; }   /* to 0..359 */
// Sets the pan of one of a stream's six sub-groups and marks it dirty. (inferred)
void SetStreamGroupPan(STREAMSND *sound, unsigned int group, int deg)
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
} } // namespace M_func_001FC380

namespace M_func_001FC408 { extern "C" {
struct R { float lo, hi; };
struct E { int object; struct R range; float gain; };
struct P { char pad[0xA0]; int groupSourceCounts[6]; struct E groupSources[6][8]; };
void SetSndFlags(struct P *, int, int);
// Adds a positional source (object, volume, range) to a stream sub-group (at most 8 per group); the loudest audible
// source drives the group's volume and pan. (inferred)
void AddStreamGroupSource(struct P *sound, unsigned int group, int object, struct R *range, float gain)
{
    if (group < 6 && sound->groupSourceCounts[group] < 8) {
        struct E *source;
        SetSndFlags(sound, SND_POSITIONAL, SND_POSITIONAL);
        source = &sound->groupSources[group][sound->groupSourceCounts[group]++];
        source->object = object;
        source->gain = gain;
        source->range = *range;
        if (source->range.lo == source->range.hi)
            source->range.hi += 1.0f;   /* keep the falloff range non-empty */
    }
}
} } // namespace M_func_001FC408

namespace M_func_001FC4F8 { extern "C" {
// Kind of a streamed sound (slot 8): 2 when looping, else 1. (inferred)
int GetStreamSoundKind(STREAMSND *sound) { return (sound->streamKind & 4) ? 2 : 1; }
} } // namespace M_func_001FC4F8

namespace M_PlayStreamsnd { extern "C" {
struct K { int keyLo; int keyHi; int kind; char filename; char pad[0x110 - 0xD]; };   /* a file lookup key (as hud.cpp's) */
/* the stream's file record: key words at +0, +4, +0x34, +0x3C; a dialog stream (SNDSTREAM) has its headset flag at +0x4C */
struct X { int key0; int key1; char pad[0x34 - 8]; int key2; int unknown38; int key3; char pad2[0x4C - 0x40]; int excludeFromHeadset; };
struct VTE { short d; short p; void (*pfn)(void *); };
struct P { char pad[0xC]; struct VTE *vtable; char pad1[0x14 - 0x10]; int panAngle; char pad2[0x44 - 0x18]; float effectiveVolume; char pad3[0x52 - 0x48]; short playState; char pad4[0x60 - 0x54]; struct X *streamRecord; char pad5[0x3B8 - 0x64]; int streamKind; };
/* the 'SUKR' start command's 0x18-byte payload */
struct M { int fileKey; int volume; int pan; int channel; int kind; int playOnHeadset; };
extern SNDMGR D_003DD040;
void InitFileKey(struct K *);
int GetStreamVolumeChannel(SNDMGR *, struct X *);
void PauseSnd(struct P *);
void SetSndFlags(struct P *, int, int);
void OnStreamSoundStarted(void);
void func_00233130(int, int, int, struct M *, void (*)(void), struct P *);
static inline int BuildStreamFileKey(struct K *key, struct X *record)
{
    if (!record) return 0;
    {
        int k2 = record->key2;
        int k0 = record->key0;
        int k3 = record->key3;
        int k1 = record->key1;
        key->kind = 2;
        key->keyHi = k1 ^ k3;
        key->keyLo = (k0 ^ k2) + 0xCA0;
    }
    return key->keyHi != 0;
}
// Starts a streamed sound (slot 9): resolves the stream's file and sends the start command to the IOP stream module
// with volume and pan.
void PlayStreamsnd(struct P *sound)
{
    struct K key;
    struct M message;
    int channel;
    key.kind = 2;
    InitFileKey(&key);
    channel = GetStreamVolumeChannel(&D_003DD040, sound->streamRecord);
    if (BuildStreamFileKey(&key, sound->streamRecord)) {
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
                playOnHeadset = sound->streamRecord->excludeFromHeadset == 0;
            message.playOnHeadset = playOnHeadset;
        }
        sound->playState = 1;
        sound->vtable[1].pfn((char *)sound + sound->vtable[1].d);   /* Incref for the callback */
        func_00233130(SUKR_RPC, SUKR_START, 0x18, &message, OnStreamSoundStarted, sound);
        SetSndFlags(sound, SND_START | SND_PAUSE | SND_CONTINUE | SND_STOP | SND_DIRTY_VOLUME | SND_DIRTY_PAN, 0);
    } else {
        SetSndFlags(sound, SND_ALL, 0);
    }
}
} } // namespace M_PlayStreamsnd

// Stops a streamed sound on the IOP (slot 10). (inferred)
extern "C" void StopStreamSound(SNDV *sound)
{
    func_00233088(SUKR_RPC, SUKR_STOP, sound->voiceHandle, 0, 0, 0, 0);
}

// Pauses a streamed sound on the IOP (slot 11). (inferred)
extern "C" void PauseStreamSound(SNDV *sound)
{
    func_00233088(SUKR_RPC, SUKR_PAUSE, sound->voiceHandle, 0, 0, 0, 0);
}

// Resumes a streamed sound on the IOP (slot 12). (inferred)
extern "C" void ResumeStreamSound(SNDV *sound)
{
    func_00233088(SUKR_RPC, SUKR_CONTINUE, sound->voiceHandle, 0, 0, 0, 0);
}

namespace M_ApplyStreamsndParams { extern "C" {
struct P { char pad[0x14]; int panAngle; float pitchRatio; char pad2[0x40 - 0x1C]; int voiceHandle; float effectiveVolume; char pad3[0x50 - 0x48]; unsigned short flags;
    char pad4[0x70 - 0x52]; float groupVolumes[6]; int groupPans[6]; char pad5[0x3BC - 0xA0]; int dirtyGroups; };
void func_00233088(int, int, int, int, int, int, int);
// Sends a stream's changed volume, pan and pitch, then each sub-group's parameters (slot 13).
void ApplyStreamsndParams(struct P *sound)
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
        func_00233088(SUKR_RPC, SUKR_SET_PARAMS, sound->voiceHandle, changed, volume, sound->panAngle, pitch);
    }
    if (sound->flags & SND_DIRTY_GROUP_PARAMS) {
        for (i = 0; i < 6; i++) {
            int mask = 1 << i;
            if (sound->dirtyGroups & mask)
                func_00233088(SUKR_RPC, SUKR_SET_GROUP_PARAMS, sound->voiceHandle, i, (int)(sound->groupVolumes[i] * 1024.0f), sound->groupPans[i], 0);
        }
        sound->dirtyGroups = 0;
    }
}
} } // namespace M_ApplyStreamsndParams

namespace M_func_001FC878 { extern "C" {
struct VTE { short d; short pad; void (*pfn)(void *); };
struct P { char pad[0xC]; struct VTE *vtable; char pad2[0x40 - 0x10]; int voiceHandle; char pad3[0x4E - 0x44]; short pauseCount; unsigned short flags; unsigned short playState; char pad4[0x68 - 0x54]; long playTime; };
extern SNDMGR D_003DD040;
void OnStreamsndStatus(void);
void OnBankSoundStatus(void);
void func_002330D0(int, int, int, int, int, int, int, void (*)(void), long);
void SetSndFlags(struct P *, int, int);
// Queries a stream's play position and whether it is still playing (slot 14), with callbacks; accumulates paused time.
// (inferred)
void QueryStreamSoundStatus(struct P *sound)
{
    if (sound->flags & SND_QUERY_STARTED) {
        sound->vtable[1].pfn((char *)sound + sound->vtable[1].d);   /* Incref for the callback */
        func_002330D0(SUKR_RPC, SUKR_QUERY_PLAYING, sound->voiceHandle, 0, 0, 0, 0, OnStreamsndStatus, (long)sound);
        SetSndFlags(sound, SND_QUERY_STARTED, 0);
    }
    if (sound->playState)
        return;
    if (sound->flags & SND_QUERY_PLAYING) {
        sound->vtable[1].pfn((char *)sound + sound->vtable[1].d);
        func_002330D0(SUKR_RPC, SUKR_QUERY_POSITION, sound->voiceHandle, 0, 0, 0, 0, OnBankSoundStatus, (long)sound);
        SetSndFlags(sound, SND_QUERY_PLAYING, 0);
    }
    if ((sound->flags & SND_TIMED) && sound->pauseCount <= 0)   /* playing: advance the play time */
        sound->playTime += D_003DD040.tickNow - D_003DD040.tickLast;
}
} } // namespace M_func_001FC878

/* STREAMSND slot 15: streams are not culled */
// Culling hook (slot 15): nothing, streams are never culled. (inferred)
extern "C" void SetStreamSoundCulled()
{
}

namespace M_UpdateStreamsndPositional { extern "C" {
typedef int qword __attribute__((mode(TI)));
struct LO {char pad28[0x28];unsigned long flags;};
struct RANGE {float low,high;};
struct E {struct LO *object;struct RANGE limits;float gain;};
struct P {char pad1C[0x1C];struct LO *object;qword position;char pad30[0x70];int groupSourceCounts[6];struct E groupSources[6][8];};
extern qword D_002A2DD0;
int AreFloatsNear__Ffff(float,float,float);
void UpdateSndPositional(struct P *);
qword GetLoWorldPos(struct LO *);
void CalculateSndVolPan(struct P *,qword,const struct RANGE *,float *,int *);
void SetStreamGroupPan(struct P *,int,int);
void SetStreamsndGroupVolume(struct P *,int,float);
static inline int AreVectorsNear(qword a, qword b)
{
    return AreFloatsNear__Ffff(QwordX(a), QwordX(b), 0.0001f) && AreFloatsNear__Ffff(QwordY(a), QwordY(b), 0.0001f) && AreFloatsNear__Ffff(QwordZ(a), QwordZ(b), 0.0001f);
}
// Positional update of a stream (slot 16): the base update, then per sub-group the loudest audible source sets the
// group's volume and pan.
void UpdateStreamsndPositional(struct P *sound)
{
    int group;
    if (sound->object || !AreVectorsNear(D_002A2DD0, sound->position))
        UpdateSndPositional(sound);
    for (group = 0; group < 6; group++) {
        int count = sound->groupSourceCounts[group];
        if (count) {
            float maximum = 0.0f;
            int panAngle = 0, numActive = 0, i;
            int *panOut = &panAngle;   /* only the first audible source's pan is computed */
            float fade;
            for (i = 0; i < count; i++) {
                struct E *entry = &sound->groupSources[group][i];
                struct LO *object = entry->object;
                if (object && ((object->flags >> 60) & 1) == 1) {   /* in the world */
                    numActive++;
                    CalculateSndVolPan(sound, GetLoWorldPos(object), &entry->limits, &fade, panOut);
                    panOut = 0;
                    { float gain = fade * entry->gain; maximum = maximum > gain ? maximum : gain; }
                }
            }
            SetStreamGroupPan(sound, group, numActive == 1 ? panAngle : 0);
            SetStreamsndGroupVolume(sound, group, maximum);
        }
    }
}
} } // namespace M_UpdateStreamsndPositional

namespace M_func_001FCBA8 { extern "C" {
struct VTE { short d; short p; void (*pfn)(void *); };
struct VT { struct VTE a[8]; };
struct X { char pad[0xC]; struct VT *vtable; char pad2[0x40 - 0x10]; int voiceHandle; char pad3[0x50 - 0x44]; unsigned short flags; short playState; };
void ResumeSnd(struct X *);
void SetSndFlags(struct X *, int, int);
#define VF(q, n) (q)->vtable->a[n].pfn((char *)(q) + (q)->vtable->a[n].d)
// Async callback when a stream started: stores its stream handle, or stops the sound if starting failed; drops the
// callback's reference. (inferred)
void OnStreamSoundStarted(int handle, long context)
{
    struct X *sound = (struct X *)context;
    sound->voiceHandle = handle;
    if (handle)
        SetSndFlags(sound, SND_QUERY_STARTED, SND_QUERY_STARTED);
    else {
        ResumeSnd(sound);
        sound->playState = 0;
        if (!(sound->flags & SND_STOP))
            VF(sound, 7);   /* slot 7 */
    }
    VF(sound, 2);   /* Decref */
}
} } // namespace M_func_001FCBA8
#undef VF

namespace M_OnStreamsndStatus { extern "C" {
struct VT { int pad[4]; short d; short p; void (*pfn)(void *); };
struct X { char pad[0xC]; struct VT *vtable; char pad2[0x52 - 0x10]; short playState; };
void ResumeSnd(struct X *);
void SetSndFlags(struct X *, int, int);
// Async callback of the stream status query.
void OnStreamsndStatus(int hasStopped, long context)
{
    struct X *sound = (struct X *)context;
    if (hasStopped) {
        sound->playState = 0;
        ResumeSnd(sound);
    } else {
        SetSndFlags(sound, SND_QUERY_STARTED, SND_QUERY_STARTED);   /* ask again */
    }
    sound->vtable->pfn((char *)sound + sound->vtable->d);   /* Decref */
}
} } // namespace M_OnStreamsndStatus

// Adds a reference to a streamed sound (slot 1).
void STREAMSND::AddRef()   /* key function: STREAMSND's vtable comes out here */
{
    SNDV *sound = (SNDV *)this;
    sound->refCount++;
}

namespace M_DecrefStreamsnd { extern "C" {
void DestroyBasic(STREAMSND *, int);
void FreeStreamsnd(STREAMSND *);
// Drops a reference to a streamed sound (slot 2); at zero it is destroyed and returned to its pool. (inferred)
void DecrefStreamsnd(STREAMSND *sound)
{
    if (--sound->refCount == 0 && sound) {
        DestroyBasic(sound, 0);
        FreeStreamsnd(sound);
    }
}
} } // namespace M_DecrefStreamsnd

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

namespace M_SNDMGR_ctor { extern "C" {
struct T { float fastSpeed; float slowSpeed; float fastTime; };   /* SMP: SmoothValue parameters */
struct V2 { float x, y; };
/* SNDMGR (layout: sly2/sndmgr.h): BASIC's header, the headset option, the bank / stream sound pools, the dirty list, the
   paused flag, the mute-when-stopped flags, the microphone noise curves (CLQ), the playback mode, the music layers'
   smoothing parameters, the counted sound pointers released by the destructor (dialog +0x200, the eight music-stack
   entries +0x208, ambience +0x228). */
struct P {
    int frame; int sidebag; int pad8; void *vtable; int isHeadsetEnabled; char pad14[0x1C - 0x14]; char bankSoundPool[0x18]; char streamSoundPool[0x18]; char dirtySounds[0xC];
    int isPaused; char pad5C[0x68 - 0x5C]; int muteWhenStoppedFlags; char pad6C[0x80 - 0x6C]; char micNoiseRangeCurve[0x10]; char micNoiseHeightCurve[0x10];
    char padA0[0x104 - 0xA0]; int playbackMode; char pad108[0x114 - 0x108]; struct T musicLayerSmoothing[3];
    char pad138[0x200 - 0x138]; int dialogSound; int musicStackCount; int musicStack[8]; int ambienceSound;
};
extern char D_002CA510[] __asm__("_vt$6CID152");
extern const struct V2 D_002A83B8;
extern const struct V2 D_002A83C0;
void InitSlotheap(void *, int, int, int);
void InitDl__FP2DLi(void *, int);
void FitClq__FffffP3CLQ(float, float, float, void *, float);
static inline void InitSmoothing(struct T *smoothing, float fastSpeed, float fastTime)
{
    smoothing->fastSpeed = fastSpeed;
    smoothing->slowSpeed = 0.0f;
    smoothing->fastTime = fastTime;
}
// Constructs the sound manager (g_sndmgr, at static initialisation): the bank and stream sound pools, empty counted
// pointers, the dirty list, the music layers' smoothing and the microphone noise curves.
struct P *SNDMGR_ctor(struct P *soundManager)
{
    int n;
    int *entry;
    soundManager->frame = 0;
    soundManager->sidebag = 0;
    soundManager->vtable = D_002CA510;
    InitSlotheap(soundManager->bankSoundPool, 0x70, 0x80, 0x10);      /* 128 bank sounds of 0x70 bytes */
    InitSlotheap(soundManager->streamSoundPool, 0x3C0, 0x10, 0x10);   /* 16 stream sounds of 0x3C0 bytes */
    soundManager->dialogSound = 0;
    entry = soundManager->musicStack;
    for (n = 7; n != -1; n--) *entry++ = 0;
    soundManager->ambienceSound = 0;
    InitDl__FP2DLi(soundManager->dirtySounds, 0x38);
    soundManager->muteWhenStoppedFlags = MUTE_WHEN_STOPPED_ALL;
    soundManager->isHeadsetEnabled = 0;
    soundManager->playbackMode = 0;
    InitSmoothing(&soundManager->musicLayerSmoothing[2], 1.0f, 0.25f);
    InitSmoothing(&soundManager->musicLayerSmoothing[1], 1.0f, 0.25f);
    {
        /* quadratics through (0, 0), (0.5, x), (1, y): noise range 500 / 3000 and height 500 / 1000 at half / full level */
        struct T *smoothing = &soundManager->musicLayerSmoothing[0];
        float zero;
        float half = 0.5f;
        InitSmoothing(smoothing, 3.0f, 0.25f);
        zero = smoothing->slowSpeed;
        FitClq__FffffP3CLQ(zero, D_002A83B8.y, half, soundManager->micNoiseRangeCurve, D_002A83B8.x);
        FitClq__FffffP3CLQ(zero, D_002A83C0.y, half, soundManager->micNoiseHeightCurve, D_002A83C0.x);
    }
    soundManager->isPaused = 0;
    return soundManager;
}
} } // namespace M_SNDMGR_ctor

namespace M_StartupSound { extern "C" {
/* SNDMGR: the volume table (8 rows x 4 channels; row 2 is read as the stream channel volumes), the master volume,
   four channel volumes, this and the last update's tick */
extern SNDMGR D_003DD040;
struct CLOCK;
extern struct CLOCK D_002DDED0;
void snd_StartSoundSystem(int);
int func_00233048(int, int, int, int, int, int, int);
long TickNow(struct CLOCK *);
// Boot-time sound setup: starts the 989snd system and the IOP stream module, sets all volumes to 1 and records the start
// time.
void StartupSound(void)
{
    int channel, row;
    long now;
    snd_StartSoundSystem(2);
    (void)func_00233048(SUKR_RPC, SUKR_INIT, 0, 0, 0, 0, 0);
    D_003DD040.masterVolume = 1.0f;
    for (channel = 0; channel < 4; channel++) {
        D_003DD040.channelVolumes[channel] = 1.0f;
        for (row = 0; row < 8; row++)
            D_003DD040.volumes[row][channel] = 1.0f;
    }
    now = TickNow(&D_002DDED0);
    D_003DD040.tickNow = now;
    D_003DD040.tickLast = now;
}
} } // namespace M_StartupSound

namespace M_func_001FCF70 { extern "C" {
struct S { char pad[0x6C]; int isMicLevelQueryBusy; int micLevel; };   /* SNDMGR (micLevel: a float, stored as its bits) */
// Async callback of the headset microphone level query (SUKR command 0xB, UpdateHeadsetMicrophone): stores the level and
// clears the busy flag. (inferred)
void OnMicLevelResult(int result, long context) { struct S *soundManager = (struct S *)(int)context; soundManager->micLevel = result; soundManager->isMicLevelQueryBusy = 0; }
} } // namespace M_func_001FCF70

namespace M_func_001FCF88 { extern "C" {
// Async callback of the manager's periodic status query: stores the result and clears the busy flag. (inferred)
void OnSoundManagerStatusResult(int result, long context) { SNDMGR *soundManager = (SNDMGR *)(int)context; soundManager->statusResult = result; soundManager->isStatusQueryBusy = 0; }
} } // namespace M_func_001FCF88

namespace M_func_001FCFA0 { extern "C" {
// The record of the dialog line with the given id (0 when the level has none).
SNDSTREAM *FindDialogStream(SNDMGR *soundManager, int id)
{
    SNDSTREAM *stream;
    for (stream = soundManager->firstDialogStream; stream; stream = stream->next)
        if (stream->id == id)
            return stream;
    return 0;
}
} } // namespace M_func_001FCFA0

// Duration of a dialog line's stream by id (0 when unknown); subtitles use it to pace the typewriter effect. (inferred)
extern "C" float GetDialogStreamDuration(int id)
{
    STREAMREC *prec = FindDialogStream(id);
    float g = 0.0f;
    if (prec != 0) {
        g = prec->duration;
    }
    return g;
}

namespace M_func_001FCFF8 { extern "C" {
// The file record of the music track with the given id (0 when the level has none); PushMusic streams it.
SNDFILE *FindMusicStream(SNDMGR *soundManager, int id)
{
    SNDFILE *file;
    for (file = soundManager->firstMusicFile; file; file = file->next)
        if (file->id == id)
            return file;
    return 0;
}
} } // namespace M_func_001FCFF8

namespace M_PsndFromSfxid { extern "C" {
// The manager's sound entry for a sound-effect id, or 0 when the level does not have it.
int *FindSoundForSfxId(SNDMGR *soundManager, int sfxId)
{
    int index;
    if (sfxId < 0 || sfxId >= soundManager->numSfx)
        return 0;
    index = soundManager->soundIndexFromSfx[sfxId];
    if (index < 0)
        return 0;
    return &soundManager->soundKinds[index];
}
} } // namespace M_PsndFromSfxid

// Index of a sound entry in the manager's table.
extern "C" int GetSoundIndex(SNDMGR *soundManager, SNDV **ppsnd)
{
    return (int)((char *)ppsnd - (char *)soundManager->soundKinds) >> 2;
}

namespace M_func_001FD080 { extern "C" {
struct T { char pad[0x44]; struct T *next; };
struct S { char pad[0xF0]; struct T *firstMusicFile; struct T *ambienceRecord; };   /* SNDMGR */
// Which volume channel a stream belongs to: 1 for a music track, 0 for the ambience stream, 2 for anything else (the
// dialog streams). SetSndmgrChannelVolume confirms the mapping: channel 0 drives the ambience sound, 1 the music stack,
// 2 the dialog sound.
int GetStreamVolumeChannel(struct S *soundManager, struct T *record)
{
    struct T *file;
    for (file = soundManager->firstMusicFile; file; file = file->next)
        if (file == record)
            return 1;
    return soundManager->ambienceRecord != record ? 2 : 0;
}
} } // namespace M_func_001FD080

namespace M_func_001FD0C0 { extern "C" {
extern int D_002C3C40[];
// OID of the state machine that animates dialog speaker role n (n < 6), or -1. (inferred)
int GetDialogRoleSmOid(int unused, unsigned int role) { if (role >= 6) return -1; return D_002C3C40[role]; }
} } // namespace M_func_001FD0C0

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
class CBinaryStreamBase
{
public:
    int m_grfbis;
    byte *m_pb;
    int m_cb;
    int m_ibStream;
    byte *m_pbRaw;
    int m_cbRaw;
    int m_grfDecomp;
    int m_cbSpillOver;
    int m_unk20;
    byte *m_abWindow;
    int m_unk28;
    int m_unk2c;
};
class CBinaryInputStream : public CBinaryStreamBase
{
public:
    int m_cbSpool;
    byte *m_abSpool;
    int m_fd;
    int m_cbDone;
    int m_cbRequest;
    int m_cbRemaining;
    int m_cbRetry;
    int m_isector;
    int m_cbuf;
    int m_cbufFill;
    int m_icbufRead;
    int m_icbufWrite;
    void Read(int cb, void *pv) __asm__("Read__18CBinaryInputStreamiPv");
    uchar U8Read() __asm__("U8Read__18CBinaryInputStream");
    char S8Read() __asm__("S8Read__18CBinaryInputStream");
    short S16Read() __asm__("S16Read__18CBinaryInputStream");
    int S32Read() __asm__("S32Read__18CBinaryInputStream");
    float F32Read() __asm__("F32Read__18CBinaryInputStream");
    void ReadFileRecord(void *pv) __asm__("CBinaryInputStream_ReadCb64");
};
struct SWALLOC
{
    int cb;
    int cbAlign;
};
void *PvAllocSwImpl(SWALLOC *pswalloc) __asm__("AllocFromWorldHeap");
void *PvAllocSwClearImpl(SWALLOC *pswalloc) __asm__("AllocClearedFromWorldHeap");
inline void *AllocWorldHeapBlock(int cb, int cbAlign)
{
    SWALLOC swalloc;
    swalloc.cb = cb;
    SWALLOC *pswalloc = &swalloc;
    pswalloc->cbAlign = cbAlign;
    return PvAllocSwImpl(pswalloc);
}
inline void *AllocClearedWorldHeapBlock(int cb, int cbAlign)
{
    SWALLOC swalloc;
    swalloc.cb = cb;
    SWALLOC *pswalloc = &swalloc;
    pswalloc->cbAlign = cbAlign;
    return PvAllocSwClearImpl(pswalloc);
}
struct STREAMSND;
extern "C" { extern int D_002E1E54; }
extern "C" void snd_UnloadBank(int bank);
extern "C" int snd_FlushSoundCommands(void);
extern "C" void *func_002333B4(void *, int, uint) __asm__("memset");
extern "C" STREAMSND *PstreamsndNew(FILEREC *, int);
extern "C" void AssignSoundRef(void *, STREAMSND *);
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
extern "C" void LoadSoundSectionFromBrx(SNDMGR *soundManager, CBinaryInputStream *input)
{
    FILEREC bankRecord;
    input->ReadFileRecord(&bankRecord);
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
        input->ReadFileRecord(soundManager->ambienceRecord);
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
            input->ReadFileRecord(&record);
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
        dialogStream->events = (ulong *)AllocWorldHeapBlock(dialogStream->numEvents * 8, 0x10);
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
        input->ReadFileRecord(musicFile);
        musicFile->next = soundManager->firstMusicFile;
        soundManager->firstMusicFile = musicFile;
    }
}
} // namespace M_LoadSoundSectionFromBrx

namespace M_PostSoundLoad { extern "C" {
struct K { int keyLo; int keyHi; int kind; char filename; char pad[0x110 - 0xD]; };   /* a file lookup key */
struct X { int key0; int key1; char pad[0x34 - 8]; int key2; int unknown38; int key3; };   /* a file record */
struct P { char pad[0xA8]; struct X bankRecord; char pad2[0xE8 - 0xA8 - sizeof(struct X)]; int bank; char pad3[0xF8 - 0xEC]; int numSounds; int *soundKinds; };   /* SNDMGR */
void InitFileKey(struct K *);
int snd_BankLoadByLoc(int, int);
void snd_ResolveBankXREFS(void);
int snd_IsSoundALooper(int, int);
/* SOUND_KIND_LOOPING: defined with PlayBanksnd */
static inline int BuildStreamFileKey(struct K *key, struct X *record)
{
    if (!record) return 0;
    {
        int k2 = record->key2;
        int k0 = record->key0;
        int k3 = record->key3;
        int k1 = record->key1;
        key->kind = 2;
        key->keyHi = k1 ^ k3;
        key->keyLo = (k0 ^ k2) + 0xCA0;
    }
    return key->keyHi != 0;
}
// After loading: loads the sound bank if needed, resolves its cross references and marks which sounds loop.
void PostSoundLoad(struct P *soundManager)
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
} } // namespace M_PostSoundLoad

namespace M_ResetSndmgr { extern "C" {
typedef int qword __attribute__((mode(TI)));
/* SNDMGR (layout: sly2/sndmgr.h) */
struct P {
    char pad0[0x14]; int unknown14; int numSfx; char bankSoundPool[0x18]; char streamSoundPool[0x18]; char dirtySounds[0xC];
    int isPaused; int musicLayer2Requests; int musicLayer0Requests; int selectedMusicLayer; int muteWhenStoppedFlags;
    int pad6C[2]; void *micNoise; char pad78[0xEC - 0x78];
    void *firstDialogStream; void *firstMusicFile; void *ambienceRecord; int numSounds; int *soundKinds; int *soundIndexFromSfx;
    int playbackMode; float musicLayerWeights[3];
    char pad114[0x138 - 0x114]; int sfxVariationStep; char pad13C[0x1D0 - 0x13C]; int numAmbienceGroups; char pad1D4[0x200 - 0x1D4];
    int dialogSound; int musicStackCount; int musicStack[8]; int ambienceSound; void *dialogRoleObjects[6];
};
extern qword D_002A2DD0;
extern char D_004435F0[];
void snd_StopAllStreams(void);
void func_00233048(int, int, int, int, int, int, int);
int snd_FlushSoundCommands(void);
void ReplaceHeldNoise(int, qword, int, void *);
void *func_002333B4(void *, int, int) __asm__("memset");
void ClearSubtitle(void *);
void AssignSoundRef(void *, void *);
void ResetSlotheap(void *);
void ClearDl__FP2DL(void *);
void ResetSndmgrVolumes(struct P *);
// Stops every sound and stream, flushes the hardware queues, clears the manager's state and dirty list and resets volumes
// (level teardown).
void ResetSndmgr(struct P *soundManager)
{
    snd_StopAllStreams();
    func_00233048(SUKR_RPC, SUKR_STOP_ALL, 0, 0, 0, 0, 0);
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
    ResetSlotheap(soundManager->bankSoundPool);
    ResetSlotheap(soundManager->streamSoundPool);
    ClearDl__FP2DL(soundManager->dirtySounds);
    ResetSndmgrVolumes(soundManager);
}
} } // namespace M_ResetSndmgr

namespace M_UpdateSndmgr { extern "C" {
struct X;
struct Q { char pad[0x38]; struct Q *nextDirty; };   /* a sound on the dirty list */
struct K { float fastSpeed, slowSpeed, fastTime; };   /* SMP */
/* SNDMGR (layout: sly2/sndmgr.h); +0x15C is volumes[2][0], the pause-fade category's weight on the ambience channel */
struct P { char pad[0x4C]; struct Q *firstDirty; char pad1[0x58 - 0x50]; int isPaused; char pad2[0x15C - 0x5C]; float pauseFadeVolume; char pad3[0x1F0 - 0x160];
    long tickNow; long tickLast; struct X *dialogSound; int musicStackCount; struct X *musicStack[8]; struct X *ambienceSound; };
struct CLOCK { float time; float deltaTime; char pad[0x40 - 8]; float realDeltaTime; };
extern struct CLOCK D_002DDED0;
extern int D_002BD4C0;
extern struct K D_002A8288;
long TickNow(struct CLOCK *);
void UpdateHeadsetMicrophone(struct P *);
void PollSoundManagerStatus(struct P *);
void ResumeSnd(struct X *);
void PauseSnd(struct X *);
void UpdateMusicMix(struct P *);
void UpdateSoundManagerStub(struct P *);
void UpdateDialog(struct P *);
int ShouldFadeWhenTimeStops(struct P *);
float SmoothValue__FfffP3SMPPf(float, float, float, struct K *, int);
void SetSndmgrChannelVolume(struct P *, int, float, int);   /* (manager, channel, volume, category): the float travels in $f12 */
void UpdateSnd(struct Q *);
int snd_FlushSoundCommands(void);
// Per-frame sound update: timing, IOP status polls, global pause while the game is paused, music mix, dialog events,
// master fades, then flushes every dirty sound and the 989snd command queue.
void UpdateSndmgr(struct P *soundManager)
{
    struct X *music;
    float fadeVolume, targetVolume;
    struct K smoothing;
    struct Q *sound;
    soundManager->tickLast = soundManager->tickNow;
    soundManager->tickNow = TickNow(&D_002DDED0);
    UpdateHeadsetMicrophone(soundManager);
    PollSoundManagerStatus(soundManager);
    music = soundManager->musicStackCount > 0 ? *(soundManager->musicStack + soundManager->musicStackCount - 1) : 0;
    /* the long-running sounds stay paused while the disc is busy (inferred: disc errors / reads) */
    if (D_002BD4C0 == CD_STATE_IDLE) {
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
    if (ShouldFadeWhenTimeStops(soundManager) && D_002DDED0.deltaTime == 0.0f) targetVolume = 0.0f;
    fadeVolume = SmoothValue__FfffP3SMPPf(soundManager->pauseFadeVolume, targetVolume, D_002DDED0.realDeltaTime, &smoothing, 0);
    SetSndmgrChannelVolume(soundManager, VOLUME_CHANNEL_AMBIENCE, fadeVolume, VOLUME_CATEGORY_PAUSE_FADE);
    SetSndmgrChannelVolume(soundManager, VOLUME_CHANNEL_MUSIC, fadeVolume, VOLUME_CATEGORY_PAUSE_FADE);
    SetSndmgrChannelVolume(soundManager, VOLUME_CHANNEL_DIALOG, targetVolume, VOLUME_CATEGORY_PAUSE_FADE);
    for (sound = soundManager->firstDirty; sound;) {
        struct Q *next = sound->nextDirty;
        UpdateSnd(sound);
        sound = next;
    }
    snd_FlushSoundCommands();
}
} } // namespace M_UpdateSndmgr

namespace M_func_001FDA48 { extern "C" {
typedef int qword __attribute__((mode(TI)));
struct CLQ { float c0, c1, c2; float pad; };   /* c0 + c1 t + c2 t^2 */
/* NOISETYPE (noise.cpp): range +0x54, height range +0x58 (low, high) */
struct O { char pad[0x54]; float range; float heightLow; float heightHigh; };
/* SNDMGR (layout: sly2/sndmgr.h) */
struct P { char pad[0x10]; int isHeadsetEnabled; char pad2[0x6C - 0x14]; int isMicLevelQueryBusy; float micLevel; char micNoise[0xC]; struct CLQ micNoiseRangeCurve; struct CLQ micNoiseHeightCurve; };
struct SW { char pad[0x4034]; struct O *micNoiseType; };   /* the world's noise type for the headset microphone */
extern struct SW D_003DD490;
void OnMicLevelResult(void);
void func_002330D0(int, int, int, int, int, int, int, void (*)(void), long);
void *GetHeroPo(struct SW *);
int IsBasicDerivedFrom__FP5BASIC3CID(void *, int);
qword GetLoWorldPos(void *);
void ReplaceHeldNoise(struct O *, qword, void *, void *);
#define CID_STEPPLAYER 0x34
static inline float Eval(struct CLQ *curve, float t)
{
    return curve->c0 + t * (curve->c1 + t * curve->c2);
}
// Headset microphone: with the headset option on, keeps one level query (SUKR command 0xB) in flight; while the hero is
// a player and the level is above half, the hero makes a noise guards can hear, its range and height growing with the
// level (ReplaceHeldNoise; with no noise type the held noise is released). (inferred; was named UpdateSoundListener,
// UpdateSndmgrListener in the draft annotation)
void UpdateHeadsetMicrophone(struct P *soundManager)
{
    struct O *noiseType;
    qword position;
    void *hero;
    if (!soundManager->isHeadsetEnabled) {
        soundManager->micLevel = 0.0f;
    } else if (!soundManager->isMicLevelQueryBusy) {
        soundManager->isMicLevelQueryBusy = 1;
        func_002330D0(SUKR_RPC, SUKR_QUERY_MIC_LEVEL, 0, 0, 0, 0, 0, OnMicLevelResult, (long)soundManager);
    }
    noiseType = 0;
    hero = GetHeroPo(&D_003DD490);
    if (hero && IsBasicDerivedFrom__FP5BASIC3CID(hero, CID_STEPPLAYER) && 0.5f < soundManager->micLevel) {
        float height;
        noiseType = D_003DD490.micNoiseType;
        position = GetLoWorldPos(hero);
        noiseType->range = Eval(&soundManager->micNoiseRangeCurve, soundManager->micLevel);
        height = Eval(&soundManager->micNoiseHeightCurve, soundManager->micLevel);
        noiseType->heightHigh = height;
        noiseType->heightLow = -height;
    }
    ReplaceHeldNoise(noiseType, position, hero, soundManager->micNoise);
}
} } // namespace M_func_001FDA48

namespace M_func_001FDBA8 { extern "C" {
void OnSoundManagerStatusResult(void);
void func_002330D0(int id, int n, int a, int b, int c, int d, int e, void (*pfn)(void), long p);
// Issues the manager's periodic status query to the IOP, once at a time. (inferred)
void PollSoundManagerStatus(SNDMGR *soundManager)
{
    if (!soundManager->isStatusQueryBusy) {
        soundManager->isStatusQueryBusy = 1;
        func_002330D0(SUKR_RPC, SUKR_QUERY_STATUS, 0, 0, 0, 0, 0, OnSoundManagerStatusResult, (long)soundManager);
    }
}
} } // namespace M_func_001FDBA8

namespace M_func_001FDC08 {
// Select and normalize three animation blend weights.
extern "C" {
struct REF {char pad50[0x50];unsigned short flags,playState;};   /* the music STREAMSND */
struct COEF {float fastSpeed,slowSpeed,fastTime;};   /* SMP */
struct PLAYER {char pad4C8[0x4C8];int action;char pad4CC[0xBFC];int mode;char pad10CC[0x20];int state;};
/* SNDMGR (layout: sly2/sndmgr.h) */
struct P {char pad5C[0x5C];int musicLayer2Requests,musicLayer0Requests,selectedMusicLayer;char pad68[0xA0];float musicLayerWeights[3];struct COEF musicLayerSmoothing[3];char pad138[0xCC];int musicStackCount;struct REF *musicStack[8];};
extern char D_003DD490[];
struct PLAYER *GetCurrentPo(void *);
int IsBasicDerivedFrom__FP5BASIC3CID(void *,int);
float SmoothValue__FfffP3SMPPf(float,float,float,struct COEF *,float *);
void SetStreamsndGroupVolume(struct REF *,int,float);
// Picks the music layer (0, 1 or 2) from requests and the player's state (alert, hiding, actions) and cross-fades the
// three layers of the current music stream toward it, keeping their total power constant. (inferred)
void UpdateMusicMix(struct P *input)
{
    struct P *soundManager = input;
    struct REF *music;
    int i;
    float sum, selectedSquare;
    soundManager->selectedMusicLayer = 1;
    if (soundManager->musicLayer2Requests) soundManager->selectedMusicLayer = 2;
    else if (soundManager->musicLayer0Requests) soundManager->selectedMusicLayer = 0;
    else if (GetCurrentPo(D_003DD490) && IsBasicDerivedFrom__FP5BASIC3CID(GetCurrentPo(D_003DD490), 0x34)) {
        struct PLAYER *player = GetCurrentPo(D_003DD490);
        if (player->state == 3) soundManager->selectedMusicLayer = 0;
        else if (player->state == 4) soundManager->selectedMusicLayer = 2;
        else if (player->mode != -1) soundManager->selectedMusicLayer = 0;
        else if (player->action >= 0) { if (player->action < 3 || player->action == 13) soundManager->selectedMusicLayer = 0; }
    }
    /* the top of the music stack (this form matches; musicStack[count - 1] does not) */
    if (soundManager->musicStackCount > 0) music = ((struct REF **)&soundManager->musicStackCount)[soundManager->musicStackCount];
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

}
} // namespace M_func_001FDC08

// Empty per-frame hook in the release build. (inferred)
extern "C" void UpdateSoundManagerStub()
{
}

namespace M_func_001FDEC8 { extern "C" {
typedef unsigned long ulong;
/* the dialog STREAMSND */
struct X { char pad[0x40]; int voiceHandle; char pad2[0x4E - 0x44]; short pauseCount; unsigned short flags; unsigned short playState; char pad3[0x60 - 0x54]; struct S *record; char pad4[0x68 - 0x64]; ulong playTime; };
struct Ent { short framesAfterPrevious; short role; int value; };   /* a dialog event */
struct S { char pad[0x50]; float duration; int numEvents; struct Ent *events; };   /* SNDSTREAM */
/* SNDMGR (layout: sly2/sndmgr.h) */
struct P { char pad[0x200]; struct X *dialogSound; char pad2[0x244 - 0x204]; int dialogEventIndex; float dialogEventTime; float dialogElapsed; };
void StopDialog(struct P *);
void ApplyDialogEvent(struct P *, struct Ent *);
// Advances the current dialog: fires its lip-sync / animation events as the stream's play time passes them and stops the
// dialog at its end. (inferred)
void UpdateDialog(struct P *soundManager)
{
    struct X *dialog = soundManager->dialogSound;
    struct S *record;
    int hasEnded;
    if (!dialog) return;
    {
        int isStarting = dialog->playState || (dialog->flags & SND_START);
        if (isStarting) return;
    }
    dialog = soundManager->dialogSound;
    if (dialog->pauseCount > 0) return;
    record = dialog->record;
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
        struct Ent *event = &record->events[soundManager->dialogEventIndex];
        float delay = (float)event->framesAfterPrevious * 0.016666668f;
        if ((float)soundManager->dialogSound->playTime / PLAY_TIME_TICKS_PER_SECOND < soundManager->dialogEventTime + delay) return;
        ApplyDialogEvent(soundManager, event);
        soundManager->dialogEventTime += delay;
        soundManager->dialogEventIndex++;
    }
}
} } // namespace M_func_001FDEC8

namespace M_func_001FE0E8 { extern "C" {
struct E { short framesAfterPrevious; short role; union { int smState; float gesture; } value; };   /* a dialog event */
#define DIALOG_ROLE_SPEAKER 3   /* the speaking object itself; the other roles are its state machines */
void SetSoEyesClosed(void *, float);
void SetSmaGoal(void *, int);
// Applies one dialog event: a gesture for role 3, otherwise sets the goal state of the speaker role's state machine
// (lip sync). (inferred)
void ApplyDialogEvent(SNDMGR *soundManager, struct E *event)
{
    int role = event->role;
    if (role != -1 && soundManager->dialogRoleObjects[role]) {
        if (role == DIALOG_ROLE_SPEAKER) SetSoEyesClosed(soundManager->dialogRoleObjects[DIALOG_ROLE_SPEAKER], event->value.gesture);
        else if (event->value.smState != -1) SetSmaGoal(soundManager->dialogRoleObjects[role], event->value.smState);
    }
}
} } // namespace M_func_001FE0E8

namespace M_PushMusic {
struct VTE { short d; short p; void (*pfn)(void *); };
struct VT { VTE a[8]; };
struct X { char pad[0xC]; VT *vtable; };   /* a STREAMSND */
struct Ref;
extern "C" void InitSoundRef(Ref *, X *);
extern "C" void AssignSoundRef(Ref *, X *);
struct Ref {
    X *px;
    Ref(X *sound) { InitSoundRef(this, sound); }
    Ref(const Ref &r) { InitSoundRef(this, r.px); }
    Ref &operator=(const Ref &r) { AssignSoundRef(this, r.px); return *this; }
    Ref &operator=(X *sound) { AssignSoundRef(this, sound); return *this; }
    ~Ref() { if (px) px->vtable->a[2].pfn((char *)px + px->vtable->a[2].d); }
};
struct S { int count; Ref entries[8]; };   /* the music stack */
struct P { char pad[0x204]; S musicStack; };   /* SNDMGR */
extern "C" { extern int D_002DD5BC; }   /* Bentley */
extern "C" { extern int D_002F7900; }   /* Murray */
extern "C" { extern int D_002E1E40; }   /* Sly (g_pjt) */
extern "C" {
Ref FindMusic(P *, int);
void StopSnd(X *, float);
X *FindMusicStream(P *, int);
X *PstreamsndNew(X *, int);
void AddStreamGroupSource(X *, int, int, float, float *);   /* (sound, group, object, gain, range): the float travels in $f12 */
}
struct RANGE { float near, far; };
static inline void Set(RANGE *range, float near, float far) { range->near = near; range->far = far; }
#define STREAM_KIND_LOOPING 4   /* STREAMSND kind bit 2 (GetStreamSoundKind) */
#define MUSIC_STACK_DEPTH 8
// Starts a music track: stops the current one, then either restarts the track if it is already on the stack (moving it to
// the top) or creates a looping stream for it whose sub-groups 3 and 4 follow Bentley / Murray and Sly (positional
// layers, 2000..5000 units), and pushes it on the 8-deep music stack. Returns the stream (0: unknown id or full stack).
extern "C" X *PushMusic(P *soundManager, int id)
{
    Ref music = FindMusic(soundManager, id);
    X *current = soundManager->musicStack.count > 0 ? (soundManager->musicStack.entries + soundManager->musicStack.count - 1)->px : 0;
    if (current) StopSnd(current, 0.0f);
    if (music.px) {
        music.px->vtable->a[7].pfn((char *)music.px + music.px->vtable->a[7].d);   /* Start */
    } else {
        X *file = FindMusicStream(soundManager, id);
        if (!file || soundManager->musicStack.count == MUSIC_STACK_DEPTH) return 0;
        float gain = 1.0f;
        music = PstreamsndNew(file, STREAM_KIND_LOOPING);
        RANGE range;
        Set(&range, 2000.0f, 5000.0f);
        AddStreamGroupSource(music.px, 3, D_002DD5BC, gain, &range.near);
        AddStreamGroupSource(music.px, 3, D_002F7900, gain, &range.near);
        AddStreamGroupSource(music.px, 4, D_002E1E40, gain, &range.near);
    }
    return (soundManager->musicStack.entries[soundManager->musicStack.count++] = music).px;
}
} // namespace M_PushMusic

namespace M_func_001FE310 {
struct VTE { short d; short p; void (*pfn)(void *); };
struct VT { VTE a[8]; };
struct X { char pad[0xC]; VT *vtable; char pad2[0x40 - 0x10]; int voiceHandle; char pad3[0x50 - 0x44]; unsigned short flags; unsigned short playState; };   /* a STREAMSND */
struct S { int count; void *entries[8]; };   /* the music stack */
struct P { char pad[0x204]; S musicStack; };   /* SNDMGR */
struct Ref {
    X *px;
    ~Ref() { if (px) px->vtable->a[2].pfn((char *)px + px->vtable->a[2].d); }
};
extern "C" void FindMusic(Ref *, P *, int);
extern "C" void StopSnd(X *, float);
extern "C" void AssignSoundRef(Ref *, void *);
// Stops a music track by id: takes it off the stack (FindMusic) and, unless it had already stopped, stops it and
// restarts the track now on top of the stack. (inferred)
extern "C" void StopMusic(P *soundManager, int id)
{
    Ref music;
    FindMusic(&music, soundManager, id);
    if (music.px) {
        /* stopped: no voice and not starting */
        int isStopped = 0;
        if (!music.px->voiceHandle) { isStopped = music.px->playState || (music.px->flags & SND_START); isStopped ^= 1; }
        if (!isStopped) {
            StopSnd(music.px, 0.0f);
            AssignSoundRef(&music, soundManager->musicStack.count > 0 ? *(soundManager->musicStack.entries + soundManager->musicStack.count - 1) : 0);
            if (music.px) music.px->vtable->a[7].pfn((char *)music.px + music.px->vtable->a[7].d);   /* Start */
        }
    }
}
} // namespace M_func_001FE310

namespace M_TMusicCur { extern "C" {
typedef unsigned long ulong;
struct Q { char pad[0x68]; ulong playTime; };   /* a STREAMSND */
struct S { int count; struct Q *entries[8]; };   /* the music stack */
struct P { char pad[0x204]; struct S musicStack; };   /* SNDMGR */
// Play time of the current music track in seconds.
float GetMusicTime(struct P *soundManager)
{
    struct Q *music;
    if (soundManager->musicStack.count > 0) { music = *(soundManager->musicStack.entries + soundManager->musicStack.count - 1); }
    else music = 0;
    if (!music) return 0.0f;
    return (float)music->playTime * (1.0f / PLAY_TIME_TICKS_PER_SECOND);
}
} } // namespace M_TMusicCur

namespace M_func_001FE478 {
struct VTE { short d; short p; void (*pfn)(void *); };
struct VT { VTE a[8]; };
struct X { char pad[0xC]; VT *vtable; char pad2[0x60 - 0x10]; X *record; };   /* a STREAMSND and its file record */
struct Ref;
extern "C" void InitSoundRef(Ref *, X *);
extern "C" void AssignSoundRef(Ref *, X *);
struct Ref {
    X *px;
    Ref(X *sound) { InitSoundRef(this, sound); }
    Ref(const Ref &r) { InitSoundRef(this, r.px); }
    Ref &operator=(const Ref &r) { AssignSoundRef(this, r.px); return *this; }
    ~Ref() { if (px) px->vtable->a[2].pfn((char *)px + px->vtable->a[2].d); }
};
struct S { int count; Ref entries[8]; };   /* the music stack */
struct P { char pad[0x204]; S musicStack; };   /* SNDMGR */
extern "C" X *FindMusicStream(P *, int);
// Takes the stream of the music track with the given id off the music stack and returns a reference to it (null when
// the track is not on the stack); the entries above it move down. (inferred)
extern "C" Ref FindMusic(P *soundManager, int id)
{
    X *file = FindMusicStream(soundManager, id);
    int i;
    for (i = 0; i < soundManager->musicStack.count; i++)
        if (soundManager->musicStack.entries[i].px->record == file) break;
    if (i == soundManager->musicStack.count) return Ref(0);
    Ref music(soundManager->musicStack.entries[i]);
    soundManager->musicStack.count--;
    for (; i < soundManager->musicStack.count; i++) soundManager->musicStack.entries[i] = soundManager->musicStack.entries[i + 1];
    return music;
}
} // namespace M_func_001FE478

namespace M_func_001FE5B8 { extern "C" {
// Sets or clears bits of the manager's mute-when-stopped flags (bit 1: a player mode, bit 2: cutscenes clear it while
// they run, so stopping game time then keeps the music and ambience audible). (inferred)
void SetSoundManagerFlags(SNDMGR *soundManager, int mask, int isSet) { if (isSet) soundManager->muteWhenStoppedFlags |= mask; else soundManager->muteWhenStoppedFlags &= ~mask; }
} } // namespace M_func_001FE5B8

// True when both mute-when-stopped flags are set (the default): UpdateSndmgr then fades the ambience and music out
// while game time is stopped (the pause menu). Cutscenes and a player mode clear a flag to keep them playing.
extern "C" int ShouldFadeWhenTimeStops(SNDMGR *soundManager)
{
    return soundManager->muteWhenStoppedFlags == MUTE_WHEN_STOPPED_ALL;
}

namespace M_StartDialog {
struct VTE { short d; short p; void (*pfn)(void *); };
struct X { char pad[0xC]; VTE *vtable; };
struct Ref;
extern "C" void AssignSoundRef(Ref *, X *);
struct Ref {
    X *px;
    Ref &operator=(X *sound) { AssignSoundRef(this, sound); return *this; }
    operator X *() { return px; }
};
struct D { char pad[0x48]; int speakerId; char pad2[0x54 - 0x4C]; int numEvents; };   /* SNDSTREAM */
/* SNDMGR (layout: sly2/sndmgr.h) */
struct P { char pad[0x200]; Ref dialogSound; char pad2[0x22C - 0x204]; X *dialogRoleObjects[6]; int dialogEventIndex; int dialogEventTime; int dialogElapsed; };
struct G;
extern "C" { extern G D_003DD490; }
extern "C" { extern char D_004435F0[]; }
extern "C" {
void StopDialog(P *, int);
D *FindDialogStream(P *, int);
X *PstreamsndNew(D *, int);
void SetSubtitle(void *, int, int);
X *FindSwObjectByClass__FP2SWi3CIDP2LO(G *, int, int, int);
int GetDialogRoleSmOid(P *, int);
X *FindLiveSma(X *, int);
}
// Starts a dialog line: stops the current one, creates its stream, shows its subtitle (unless noSubtitle) and, when the
// line has events and a speaker, binds the six lip-sync roles: role 3 is the speaker object, the others the state
// machines GetDialogRoleSmOid names on it. Each bound object is counted (vtable slot 1). Returns the stream.
extern "C" X *StartDialog(P *soundManager, int id, int noSubtitle)
{
    D *record;
    StopDialog(soundManager, id);
    record = FindDialogStream(soundManager, id);
    if (!record) return 0;
    soundManager->dialogSound = PstreamsndNew(record, 0);
    soundManager->dialogEventIndex = 0;
    soundManager->dialogEventTime = 0;
    soundManager->dialogElapsed = 0;
    if (!noSubtitle) SetSubtitle(D_004435F0, id, record->speakerId);
    if (record->speakerId != -1 && record->numEvents) {
        register X *speaker __asm__("$19")=FindSwObjectByClass__FP2SWi3CIDP2LO(&D_003DD490, 5, record->speakerId, 0);
        if (speaker) {
            register int role __asm__("$18")=0;
            register int speakerRole __asm__("$20")=3;
            register X **slot __asm__("$16")=soundManager->dialogRoleObjects;
            for(;role<6;role++,slot++) {
                if(role==speakerRole)soundManager->dialogRoleObjects[3]=speaker;
                else *slot=FindLiveSma(speaker,GetDialogRoleSmOid(soundManager,role));
                { X *current=*slot;
                  if(current) { VTE &entry=current->vtable[1];
                    entry.pfn((char *)current+entry.d); } }
            }
        }
    }
    return soundManager->dialogSound;
}
} // namespace M_StartDialog

namespace M_StopDialog { extern "C" {
struct VTE { short d; short p; void (*pfn)(void *); };
struct VT { struct VTE a[3]; };
struct X { char pad[0xC]; struct VT *vtable; };
struct A { int a, b; };   /* a dialog event */
struct E { char pad[0x54]; int numEvents; struct A *events; };   /* SNDSTREAM */
struct Y { char pad[0x60]; struct E *record; };   /* the dialog STREAMSND */
struct Ref { struct Y *px; };
/* SNDMGR (layout: sly2/sndmgr.h) */
struct P { char pad[0x200]; struct Ref dialogSound; char pad2[0x22C - 0x204]; struct X *dialogRoleObjects[6]; int dialogEventIndex; int dialogEventTime; float dialogElapsed; };
#define SM_STATE_MOUTH_REST 0x611   /* (inferred: the speaker's idle mouth) */
void ApplyDialogEvent(struct P *, struct A *);
void SetSmaGoal(void *, int);
void StopSnd(struct Y *, float);
void AssignSoundRef(struct Ref *, void *);
void func_002333B4(void *, int, int) __asm__("memset");
static inline void Stop(struct Ref *ref)
{
    if (ref && ref->px) {
        StopSnd(ref->px, 0.0f);
        AssignSoundRef(ref, 0);
    }
}
// Stops the current dialog: flushes its remaining events, returns the speakers' mouths to rest, stops the stream and
// clears the dialog state.
void StopDialog(struct P *soundManager)
{
    int role;
    struct E *record;
    if (!soundManager->dialogSound.px) return;
    record = soundManager->dialogSound.px->record;
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
        struct X *object = soundManager->dialogRoleObjects[role];
        if (object) object->vtable->a[2].pfn((char *)object + object->vtable->a[2].d);   /* release */
    }
    func_002333B4(soundManager->dialogRoleObjects, 0, sizeof(soundManager->dialogRoleObjects));
}
} } // namespace M_StopDialog

namespace M_func_001FE848 { extern "C" {
static inline int PresentValue(void){register int r __asm__("$4");__asm__("addiu %0,$zero,%1":"=r"(r):"i"(1));return r;}
// True while a dialog (optionally the one with the given id) is playing. (inferred)
int IsDialogPlaying(SNDMGR *soundManager, int id)
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
} } // namespace M_func_001FE848

namespace M_func_001FE888 { extern "C" {
typedef unsigned long ulong;
struct X { char pad[0x40]; int voiceHandle; char pad2[0x50 - 0x44]; unsigned short flags; unsigned short playState; char pad3[0x68 - 0x54]; ulong playTime; };   /* STREAMSND */
struct P { char pad[0x200]; struct X *dialogSound; char pad2[0x24C - 0x204]; float dialogElapsed; };   /* SNDMGR */
static inline float PlaySeconds(struct X *sound) { return (float)sound->playTime * (1.0f / PLAY_TIME_TICKS_PER_SECOND); }
// Play time of the current dialog in seconds; when the stream reports none and has no voice (not started yet, or a line
// without audio), the time measured by UpdateDialog instead. (inferred)
float GetDialogTime(struct P *soundManager)
{
    if (!soundManager->dialogSound) return 0.0f;
    if (PlaySeconds(soundManager->dialogSound) == 0.0f) {
        struct X *dialog = soundManager->dialogSound;
        int isStopped = 0;
        if (!dialog->voiceHandle) { isStopped = dialog->playState || (dialog->flags & SND_START); isStopped ^= 1; }
        if (isStopped) return soundManager->dialogElapsed;
    }
    return PlaySeconds(soundManager->dialogSound);
}
} } // namespace M_func_001FE888

namespace M_func_001FE990 { extern "C" {
// Index of an id among the ambience stream's groups, or -1 (used to attach ambient emitters to groups). (inferred)
int FindStreamGroupIndex(SNDMGR *soundManager, int id)
{
    int i;
    for (i = 0; i < soundManager->numAmbienceGroups; i++)
        if (soundManager->ambienceGroupIds[i] == id) return i;
    return -1;
}
} } // namespace M_func_001FE990

// Sets the 989snd output mode (mono / stereo option) if it changed.
extern "C" void SetSndmgrPlaybackMode(SNDMGR *soundManager, int nMode)
{
    if (soundManager->playbackMode != nMode) {
        soundManager->playbackMode = nMode;
        snd_SetPlaybackMode(nMode);
    }
}

namespace M_SetSndmgrMasterVolume { extern "C" {
struct S;
extern struct S D_002BBB60;   /* the volume limits (0..1) */
float ClampToLimits__FP2LMf(struct S *, float);
void snd_SetMasterVolume(int, int);
#define SND_MASTER_VOLUME_ALL 0x10   /* 989snd: the overall master (0..3 are the per-channel masters) (inferred) */
// Sets the master volume (clamped) on the 989snd library.
void SetSndmgrMasterVolume(SNDMGR *soundManager, float volume)
{
    float clamped = ClampToLimits__FP2LMf(&D_002BBB60, volume);
    int level = (int)(clamped * 1024.0f);   /* 989snd volumes are 0..1024 */
    if ((int)(soundManager->masterVolume * 1024.0f) != level) {
        soundManager->masterVolume = clamped;
        snd_SetMasterVolume(SND_MASTER_VOLUME_ALL, level);
    }
}
} } // namespace M_SetSndmgrMasterVolume

namespace M_SetSndmgrChannelVolume {
class X;
typedef void (X::*PMF)();
struct P {char pad0[0x13C];float volumes[8][4];int pad1BC;float channelVolumes[4];char pad1D0[0x30];X *dialogSound;int musicStackCount;X *musicStack[8];X *ambienceSound;};
struct LM {float low,high;};
extern "C" { extern LM D_002BBB60; }
extern "C" { extern PMF D_002A8298,D_002A82A0; }
extern "C" float ClampToLimits__FP2LMf(LM *,float);
extern "C" void *func_002333B4(void *,int,unsigned int) __asm__("memset");
extern "C" void snd_SetMasterVolume(int,int);
static inline X *ReadReceiver(X **pp)
{
    register X *receiver __asm__("$4");
    __asm__("lw %0,%1":"=r"(receiver):"m"(*pp));
    return receiver;
}
// Sets one category's weight on a volume channel, recomputes the channel's output volume, sends it and updates the live
// streams.
extern "C" void SetSndmgrChannelVolume(P *soundManager,int channel,int category,float volume)
{
    if((unsigned int)category>=8)return;
    if(channel<0)return;
    if(channel>=4)return;
    volume=ClampToLimits__FP2LMf(&D_002BBB60,volume);
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
            case VOLUME_CHANNEL_MUSIC:for(int i=0;i<soundManager->musicStackCount;i++){X **slot=&soundManager->musicStack[i];X *music=ReadReceiver(slot);if(music){union {X *p;unsigned w;} r;r.p=music;(((X *)r.w)->*action)();}}break;
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
typedef char layout_limit[(sizeof(LM)==8)?1:-1];
typedef char layout_limit_low[((unsigned)&((LM*)0)->low==0)?1:-1];
typedef char layout_limit_high[((unsigned)&((LM*)0)->high==4)?1:-1];
typedef char layout_volumes[((unsigned)&((P*)0)->volumes==0x13C)?1:-1];
typedef char layout_volume_stride[(sizeof(((P*)0)->volumes[0])==0x10)?1:-1];
typedef char layout_channel_volumes[((unsigned)&((P*)0)->channelVolumes==0x1C0)?1:-1];
typedef char layout_dialog_sound[((unsigned)&((P*)0)->dialogSound==0x200)?1:-1];
typedef char layout_count[((unsigned)&((P*)0)->musicStackCount==0x204)?1:-1];
typedef char layout_music_stack[((unsigned)&((P*)0)->musicStack==0x208)?1:-1];
typedef char layout_ambience_sound[((unsigned)&((P*)0)->ambienceSound==0x228)?1:-1];
typedef char layout_size[(sizeof(P)==0x22C)?1:-1];
} // namespace M_SetSndmgrChannelVolume

namespace M_ResetSndmgrVolumes { extern "C" {
void SetSndmgrMasterVolume(void *, float);
void SetSndmgrChannelVolume(void *, int, int, float);
// Resets the master volume and all channel weights to 1 (category 1 excepted).
void ResetSndmgrVolumes(void *soundManager)
{
    int category, channel;
    SetSndmgrMasterVolume(soundManager, 1.0f);
    for (category = 0; category < 8; category++) {
        if (category == 1) continue;   /* the options menu's volumes (menu.cpp) are kept */
        for (channel = 0; channel < 4; channel++) SetSndmgrChannelVolume(soundManager, channel, category, 1.0f);
    }
}
} } // namespace M_ResetSndmgrVolumes

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
namespace M_func_001FEED0 { extern "C" {
/* 989snd play parameters (as in PlayBanksnd) */
struct M { int bank; int pad4; int volume; int pan; short soundIndex; short pad12; int pad14[3]; short pitch; short pad22; int pad24; int mask; int pad2C; };
#define SFX_VARIATION 0x28
int RandomIntInRange__Fii(int, int);
void *func_002333B4(void *, int, int) __asm__("memset");
short GetSoundIndex(SNDMGR *, void *);
void func_002322F0(struct M *);
// After one of the hero's own effects (PlayBanksnd), unless a music layer other than 0 is selected: a random step of the
// variation value in -12..12 (mostly +1, sometimes -5; b picks a row of a by the value, and the higher the value the
// more -5 entries the row has), then plays effect 0x28 with 127 x the value as its pitch modulation. (inferred)
void StepRandomSfxVariation(SNDMGR *soundManager)
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
    func_002322F0(&params);
    }
}
} } // namespace M_func_001FEED0

namespace M_func_001FF108 { extern "C" {
extern SNDMGR D_003DD040;
int FindStreamGroupIndex(SNDMGR *, int);
void SetStreamsndGroupVolume(void *, int, float);
void SetStreamGroupPan(void *, int, int);
// Splice command: sets the volume and pan of the ambience sub-group selected by id. (inferred)
void SetAmbienceGroupMix(int unused, int groupId, int pan, float volume)
{
    int group = FindStreamGroupIndex(&D_003DD040, groupId);
    void *ambience = D_003DD040.ambienceSound;
    SetStreamsndGroupVolume(ambience, group, volume);
    SetStreamGroupPan(ambience, group, pan);
}
} } // namespace M_func_001FF108


namespace M_func_00259B48 { extern "C" {
typedef struct { int pad[2]; short d1; short p1; void (*f1)(void *); short d2; short p2; void (*f2)(void *); } VT;
typedef struct { int pad[3]; VT *vt; } O;
inline void AssignSoundRef(O **pp, O *o)
{
    if (o) o->vt->f1((char *)o + o->vt->d1);
    if (*pp) (*pp)->vt->f2((char *)*pp + (*pp)->vt->d2);
    *pp = o;
}
} } // namespace M_func_00259B48

namespace M_func_00259BB0 { extern "C" {
typedef struct { int pad[2]; short d; short p; void (*f)(void *); } VT;
typedef struct { int pad[3]; VT *vt; } O;
inline void InitSoundRef(O **pp, O *o) { if (o) o->vt->f((char *)o + o->vt->d); *pp = o; }
} } // namespace M_func_00259BB0

// ---- Data owned by this TU, in address order (docs/guides/tu-merging.md "Data"). Defined after the functions
// so no function sees a definition (keeps the code as matched); the namespaced declarations above refer to these.
extern const LM D_002A83B8 = {500.0f, 3000.0f};
extern const LM D_002A83C0 = {500.0f, 1000.0f};
int D_002C3C40[6] = {1548, 1549, 1550, -1, 1551, 1552};
