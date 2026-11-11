// amb.cpp -- AMB, an ambient sound emitter: an XFM that starts its looping sounds when added to the world.
// Entry points are its class slots: CID43::Init, LoadAmbFromBrx, PostAmbLoad, OnAmbAdd / OnAmbRemove, plus the option
// accessors EnsureCurrentAmbSound / AddAmbSound used while loading.
// TU 0x12EB48..0x12F0F0 (10 functions, address order).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_amb
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_amb
#include "sly2/xfm.h"
#include "sly2/slotheap.h"
#include "sly2/stackscope.h"
#include "sly2/classes.h"
#include "sly2/sndmgr.h"
#include "sly2/port.h"

// 0x12EF98 StaticInit_amb and GlobalConstructors_amb: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

struct AREQV { int cb; int cbAlign; };

/* A sound the emitter plays (0x14 bytes, 8 at most). */
struct AMBSND { int soundId; float minDistance; float maxDistance; float volume; float pitch; };
/* A second list of 8 records (0x10 bytes) bound at load to sounds found by id. */
struct AMBREC { int soundId; float range[2]; float gain; };

/* The ambient sound emitter: classes.h CID43 (an XFM with the AMBSND list and the linked AMBREC list). */
typedef CID43 AMB;

extern "C" {
void InitXfm(XFM *pxfm);
void SetLoBusyKind(LO *plo, int busyk);
void *AllocClearedFromStack(AREQV *preq);
void *AllocCopyFromWorldHeap(AREQV *preq, void *pv);
void **AllocClearedFromWorldHeap(AREQV *preq);
void LoadXfmFromBrx(XFM *pxfm, void *pbis);
void PostLoLoad(LO *plo);
void *FindStreamGroupIndex(SNDMGR *psndmgr, int id);
void AddStreamGroupSource(float g, void *p, void *q, AMB *pamb, float *pv);
void OnLoAdd(LO *plo);
void OnLoRemove(LO *plo);
void *CreateSfxSound(int id);
void SetSndSourceLo(void *psnd, LO *plo);
void SetSndRange(void *psnd, float sMin, float sMax);
void SetSndVolume(void *psnd, float uVol);
void SetSndPitch(void *psnd, float uPitch);
void StopSnd(void *psnd, float g);
void RcPtr_Assign(void **pp, void *p);
void AddAmbSound(AMB *amb, int soundId, int minDistance, int maxDistance, float volume, float pitch);
}

// Initializes an ambient sound emitter (class 0x2B, slot 7): base XFM init, not busy (it needs no per-frame update).
void CID43::Init()
{
    AMB *amb = (AMB *)this;
    InitXfm(amb);
    SetLoBusyKind(amb, 0);
}

// Loads an ambient emitter (slot 14): allocates room for up to 8 sounds and 8 linked sounds, loads the XFM part (whose
// options fill the lists), then shrinks both allocations to the counts actually used. (inferred)
// Layout parse-verified: a parser following it reads every byte of 4 levels.
extern "C" void LoadAmbFromBrx(AMB *amb, void *stream)
{
    STACKSCOPE obj;
    AREQV request;
    AREQV *requestRef = &request;
    request.cb = 0xA0;
    requestRef->cbAlign = 0x10;
    amb->sounds = (AMBSND *)AllocClearedFromStack(requestRef);
    request.cb = 0x80;
    requestRef->cbAlign = 0x10;
    amb->linkedSounds = (AMBREC *)AllocClearedFromStack(requestRef);
    LoadXfmFromBrx(amb, stream);
    request.cb = amb->soundCount * 0x14;
    requestRef->cbAlign = 0x10;
    amb->sounds = (AMBSND *)AllocCopyFromWorldHeap(requestRef, amb->sounds);
    request.cb = amb->linkedCount * 0x10;
    requestRef->cbAlign = 0x10;
    amb->linkedSounds = (AMBREC *)AllocCopyFromWorldHeap(requestRef, amb->linkedSounds);
}

// After-load hook (slot 15): allocates the playing-sound handle array and, if the sound system has a stream-sound group,
// registers each linked sound with it (found by sound id; inferred: these play as streamed sounds with a range).
// Also known as: AddAmbientFalloffs (upstream readable / Sly 1 name; AddAmbientFalloffs__FP2LO).
extern "C" void PostAmbLoad(AMB *amb)
{
    AREQV request;
    void *soundGroup;
    PostLoLoad(amb);
    request.cb = amb->soundCount * (int)sizeof(void *);
    request.cbAlign = 0x10;
    amb->playingSounds = AllocClearedFromWorldHeap(&request);
    soundGroup = g_sndmgr.ambienceSound.p;
    if (soundGroup) {
        int i;
        for (i = 0; i < amb->linkedCount; i++) {
            AMBREC *link = &amb->linkedSounds[i];
            AddStreamGroupSource(link->gain, soundGroup, FindStreamGroupIndex(&g_sndmgr, link->soundId), amb, link->range);
        }
    }
}

// Called when the emitter enters the world (slot 23): starts every one of its sounds as a looping positional sound on the
// emitter with its distance range, volume and pitch, keeping a reference-counted handle to each. (inferred)
extern "C" void OnAmbAdd(AMB *amb)
{
    int i;
    OnLoAdd(amb);
    for (i = 0; i < amb->soundCount; i++) {
        AMBSND *sound = &amb->sounds[i];
        void *playing = CreateSfxSound(sound->soundId);
        SetSndSourceLo(playing, amb);
        SetSndRange(playing, sound->minDistance, sound->maxDistance);
        SetSndVolume(playing, sound->volume);
        SetSndPitch(playing, sound->pitch);
        RcPtr_Assign((void **)(i * (int)sizeof(void *) + (sptr)amb->playingSounds), playing);
    }
}

// Called when the emitter leaves the world (slot 22): stops every playing sound immediately and releases its handle.
// (inferred)
// Also known as: OnAmbientUnregistered (upstream readable / Sly 1 name; OnAmbientUnregistered__FP2LO).
extern "C" void OnAmbRemove(AMB *amb)
{
    int i;
    OnLoRemove(amb);
    for (i = 0; i < amb->soundCount; i++) {
        void **slot = (void **)((i << 2) + (int)amb->playingSounds);
        if (slot && *slot) {
            StopSnd(*slot, 0.0f);
            RcPtr_Assign(slot, 0);
        }
    }
}

// Option-table accessor: returns the sound the emitter's option setters edit, adding a default one first (no sound id,
// range 300..3000, full volume, unchanged pitch) if there is none yet. (inferred)
extern "C" AMBSND *EnsureCurrentAmbSound(AMB *amb)
{
    if (!amb->currentSound) {
        amb->currentSound = &amb->sounds[amb->soundCount];
        AddAmbSound(amb, -1, 300, 3000, 1.0f, 0.0f);
    }
    return amb->currentSound;
}

// Adds a sound (id, min/max audible distance, volume, pitch) to the emitter; at most 8, extras are ignored. (inferred)
extern "C" void AddAmbSound(AMB *amb, int soundId, int minDistance, int maxDistance, float volume, float pitch)
{
    if (amb->soundCount < 8) {
        AMBSND *sound = &amb->sounds[amb->soundCount++];
        sound->soundId = soundId;
        sound->minDistance = minDistance;
        sound->maxDistance = maxDistance;
        sound->volume = volume;
        sound->pitch = pitch;
    }
}

// Adds a linked sound record (id, range, gain) to the emitter; at most 8. No callers in the binary (inferred: an option
// setter reached only through a table).
extern "C" void AddAmbLinkedSound(AMB *amb, int soundId, int rangeMin, int rangeMax, float gain)
{
    if (amb->linkedCount < 8) {
        AMBREC *link = &amb->linkedSounds[amb->linkedCount++];
        link->soundId = soundId;
        link->range[0] = rangeMin;
        link->range[1] = rangeMax;
        link->gain = gain;
    }
}
