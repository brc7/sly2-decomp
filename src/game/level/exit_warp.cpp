// exit_warp.cpp -- EXIT, a volume that sends the player to another world (it plays the queued movies, then wipes
// to the target world), and WARP, a point the player is placed at on arrival (it moves the paired objects to their
// targets, applies its rsmgs and task states, and resets every exit). Entry points: UpdateExit (per frame), TakeExit, TriggerWarp, FindWarpByOid. TU 0x227FE8..0x228B48 (22 functions, address
// order).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_exit_warp
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_exit_warp
#include "bis.h"
#include "sly2/slotheap.h"
#include "sly2/clock.h"
#include "sly2/classes.h"
#include "sly2/sw.h"
#include "sly2/game.h"
static inline ::LO *AsLo(void *plo) { return (::LO *)plo; }   /* the LO behind an object found by oid, for its virtual calls */

// 0x2289F0 StaticInit_exit_warp and GlobalConstructors_exit_warp: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

typedef int qwordV __attribute__((mode(TI)));

/* Virtual calls used here: HandleSpliceEvent (slot 3) on the exit or warp, MatchTransform (slot 38: moves the object to
   the given one) on the hero and on the paired objects. */

struct LO;


/* The 0x40-byte header of a warp blob in the brx. */
struct WARPBLOB { int a; int b; char pad[0x34]; int c; };
struct MEMREQ { int cb; int cbAlign; };

extern char D_002F66B0[];       /* the movie player */

struct CM;
LO *FindSwObject(SW *world, int findFlags, OID oid, LO *context);
int AddOrUpdateRsmg(SW *world, RSMG *arsmg, int crsmgMax, int *pcrsmg, int d, OID e, OID f, OID g);
void CutCm(CM *pcm);
extern "C" {
int FindSwObjectsByClass(SW *world, int findFlags, int cid, LO *context, int cploMax, void *aplo);
void LoadXfmFromBrx(WARP *warp, CBinaryInputStream *stream);
void CBinaryInputStream_SkipStringSw(CBinaryInputStream *stream);
LO *GetHeroPo(SW *world);
void InitXfm(EXIT *exitVolume);
void SetLoBusyKind(EXIT *exitVolume, int busyk);
void *AllocCopyFromWorldHeap(MEMREQ *request, WARPBLOB *pblob);
void LoadVolFromBrx(EXIT *exitVolume, CBinaryInputStream *stream);
void *AllocFromWorldHeap(MEMREQ *request);
void IncrementSwHandsOff(SW *world, int n);
void ApplyTaskStates(void *p, int c, TASKSTATEREQ *ataskstate, void *object);
void RevokeCmPolicy(void *pcm, int a, int b, void *pv, int c, int d);
void AppendBoundedPair(void *p, TASKSTATEREQ *ataskstate, int cMax, int *pc, int d, int e);
void FindLevelIndexByFileRecord(void *pvWorld);
void UpdateLo(EXIT *exitVolume, float dt);
void CMpeg_QueueMovie(void *pmpeg, void *pvMovie);
void WipeToWorldWarp(SW *world, void *pvWorld, int a, int b, int grf, int c);
qwordV GetLoWorldPos(LO *object);
qwordV ConvertPosBetweenLos(int a, EXIT *exitVolume, qwordV pos);
int FindBspLeafContainingPointFast(void *pbsp, int n, qwordV pos);
void TriggerRsmg(SW *world, int crsmg, RSMG *arsmg, WARP *warp, int fOn);

WARP *FindWarpByOid(SW *world, OID oid, OID contextOid);
EXIT *FindDefaultExit(SW *world);
void LoadWarpBlobFromBrx(CBinaryInputStream *stream, void **blobOut);
void SetExitExits(EXIT *exitVolume, int newState);
void TakeExit(EXIT *exitVolume);
void AddWarpOidAlias(WARP *warp, OID oid);
}

// Finds the WARP with the given oid, or else the WARP that lists oid among its aliases. (Sly 1: PwarpFromOid.)
// Also known as: PloFindCheckpoint (upstream readable / Sly 1 name; PloFindCheckpoint__FP2SW3OIDT1).
extern "C" WARP *FindWarpByOid(SW *world, OID oid, OID contextOid)
{
    WARP *warpList[32];
    WARP *result;
    LO *context = 0;
    int c, i;
    int findFlags = 5;
    if (contextOid != -1)
    {
        context = FindSwObject(world, 5, contextOid, 0);
        if (context)
            findFlags = 1;
    }
    result = 0;
    c = FindSwObjectsByClass(world, findFlags, 0x2A, context, 0x20, warpList);
    for (i = 0; i < c; i++)
    {
        WARP *warp = warpList[i];
        int j;
        if (warp->oid == oid)
            goto found;
        for (j = 0; j < warp->aliasCount; j++)
        {
            if (warp->aliases[j] == oid)
                goto found;
        }
        continue;
    found:
        result = warp;
    }
    return result;
}

// Returns the last EXIT whose "default" flag is set, or null. (Sly 1: PexitDefault.)
extern "C" EXIT *FindDefaultExit(SW *world)
{
    EXIT *exitList[32];
    EXIT *result = 0;
    int c = FindSwObjectsByClass(&g_sw, 5, 0x1A, 0, 0x20, exitList);
    int i;
    for (i = 0; i < c; i++)
        if (exitList[i]->isDefault)
            result = exitList[i];
    return result;
}

// Loads the in-level teleport destination (cid 41) from the BRX: the XFM part, then skips a string.
// Layout parse-verified: a parser following it reads every byte of 4 levels.
void CID41::LoadFromBrx(CBinaryInputStream *stream)
{
    WARP *warp = (WARP *)this;
    LoadXfmFromBrx(warp, stream);
    CBinaryInputStream_SkipStringSw(stream);
}

// Moves the hero onto this warp point (the hero's vtable slot 38) and sends the warp's Splice trigger event (5). A Splice-callable
// method; cutscene relays call it too.
// Also known as: TriggerWarp (upstream readable / Sly 1 name; TriggerWarp__FP4WARP).
extern "C" void PutHeroAtWarp(WARP *warp)
{
    LO *hero = GetHeroPo(&g_sw);
    if (hero)
    {
        AsLo(hero)->MatchTransform(warp);
        warp->HandleSpliceEvent(5, 0, 0, 0, 0);
    }
}

// EXIT initialization (vtable slot 7): XFM init, default target warp oid 0x189 and wipe oid -1, not busy.
void EXIT::Init()
{
    EXIT *exitVolume = this;
    uptr grf;
    InitXfm(exitVolume);
    exitVolume->targetWarpOid = 0x189;
    exitVolume->wipeOid = -1;
    SetLoBusyKind(exitVolume, 0);
    grf = (uptr)exitVolume->ploupd;
    if (!(grf & 1))
        ((LOUPD *)grf)->grf |= 2;
    else
        exitVolume->unknown40 = grf | 2;
}

// Reads a 0x40-byte warp blob header from the BRX and allocates a copy when it is not empty (the exit's target world).
// Layout parse-verified: a parser following it reads every byte of 4 levels.
// Also known as: LoadKeyRecordFromBrx (upstream readable / Sly 1 name;
//   LoadKeyRecordFromBrx__FP18CBinaryInputStreamPUc).
extern "C" void LoadWarpBlobFromBrx(CBinaryInputStream *stream, void **blobOut)
{
    WARPBLOB blob;
    MEMREQ memreq;
    stream->Read(0x40, &blob);
    if (blob.b != blob.c)
    {
        {
            MEMREQ *request = &memreq;
            request->cb = 0x40;
            request->cbAlign = 0x10;
        }
        *blobOut = AllocCopyFromWorldHeap(&memreq, &blob);
    }
    else
        *blobOut = 0;
}

// Loads an EXIT from the BRX: the VOL part, its target-world blob and the list of movies played before the wipe.
// Layout parse-verified: a parser following it reads every byte of 4 levels.
// Also known as: LoadShapeKeysFromBrx (upstream readable / Sly 1 name;
//   LoadShapeKeysFromBrx__FP2LOP18CBinaryInputStream).
extern "C" void LoadExitFromBrx(EXIT *exitVolume, CBinaryInputStream *stream)
{
    int i = 0;
    int c;
    MEMREQ memreq;
    LoadVolFromBrx(exitVolume, stream);
    LoadWarpBlobFromBrx(stream, &exitVolume->targetWorld);
    c = stream->U8Read();
    memreq.cbAlign = 0x10;
    memreq.cb = c * (int)sizeof(void *);
    exitVolume->movieCount = c;
    exitVolume->movies = (void **)AllocFromWorldHeap(&memreq);
    for (; i < exitVolume->movieCount; i++)
        LoadWarpBlobFromBrx(stream, &exitVolume->movies[i]);
}

// Changes the exit state (0 player inside, 1 left, 2 off, 3 armed, 4 warping) and stamps the time; entering state 4
// takes control from the player (hands-off) and records the exit as the one being taken. (Sly 1: SetExitExits.)
// Also known as: SetExitState (upstream readable / Sly 1 name; SetExitState__FP2LOi).
extern "C" void SetExitExits(EXIT *exitVolume, int newState)
{
    if (newState != exitVolume->state)
    {
        if (newState == 4)
        {
            SetLoBusyKind(exitVolume, 1);
            IncrementSwHandsOff(&g_sw, 3);
            g_sw.pexitTaken = (EXIT *)exitVolume;
        }
        exitVolume->state = newState;
        exitVolume->stateTime = g_clock.time;
    }
}

// Takes an armed exit: a redirecting exit hands over to the default exit; otherwise applies the exit's task states,
// revokes two camera policies, switches to the warping state, schedules the wipe and sends the Splice trigger event (5).
// Also known as: TriggerExit (upstream readable / Sly 1 name; TriggerExit__FP4EXIT).
extern "C" void TakeExit(EXIT *exitVolume)
{
    if (exitVolume->state != 3)
        return;
    if (exitVolume->redirectsToDefault)
    {
        EXIT *defaultExit;
        SetExitExits(exitVolume, 0);
        defaultExit = FindDefaultExit(&g_sw);
        if (defaultExit)
            TakeExit(defaultExit);
        return;
    }
    ApplyTaskStates(&g_game, exitVolume->taskStateCount, exitVolume->taskStates, exitVolume);
    RevokeCmPolicy(&g_cm, 2, -1, &g_cm.cplook, 0, 0);
    RevokeCmPolicy(&g_cm, 2, -1, &g_cm.cpzoom, 0, 0);
    SetExitExits(exitVolume, 4);
    exitVolume->wipeTime = g_clock.time + exitVolume->wipeDelay;
    exitVolume->HandleSpliceEvent(5, 0, 0, 0, 0);
}

// Adds a task-state entry to the exit's four-entry list (applied when the exit is taken). Splice-callable; the name
// was AddExitRsmg, but the list it fills is the task-state list.
extern "C" void AddExitTaskState(EXIT *exitVolume, int d, int e)
{
    AppendBoundedPair(&g_game, exitVolume->taskStates, 4, &exitVolume->taskStateCount, d, e);
}

// Returns the index of the exit's target level in the level table (IlevelFromRec on its world blob). (inferred)
extern "C" void GetExitTargetLevelIndex(EXIT *exitVolume)
{
    FindLevelIndexByFileRecord(exitVolume->targetWorld);
}

// Per-frame EXIT update: after the LO update, tests whether the hero is inside the exit volume and steps the state
// machine (re-arms after the hero has been out for dtRearm, takes the exit when entered while armed); in the warping
// state, queues the movies and wipes to the target world when the wipe time arrives. Sly 1: UpdateExit.
// Also known as: UpdateExit (upstream readable / Sly 1 name; UpdateExit__FP2LO).
extern "C" void UpdateExit(EXIT *exitVolume, float dt)
{
    UpdateLo(exitVolume, dt);
    switch (exitVolume->state)
    {
    case 4:
        if (0.0f < exitVolume->wipeTime && exitVolume->wipeTime <= g_clock.time)
        {
            int i;
            for (i = 0; i < exitVolume->movieCount; i++)
                CMpeg_QueueMovie(D_002F66B0, exitVolume->movies[i]);
            WipeToWorldWarp(&g_sw, exitVolume->targetWorld, exitVolume->targetWarpOid, -1, exitVolume->skipWipe ? 0x20 : 0, exitVolume->wipeOid);
            exitVolume->wipeTime = 0.0f;
        }
        break;
    case 0:
    case 1:
    case 3:
    {
        LO *hero = GetHeroPo(&g_sw);
        union { qwordV q; } u;
        int isInside;
        if (!hero)
            return;
        u.q = ConvertPosBetweenLos(0, exitVolume, GetLoWorldPos(hero));
        isInside = FindBspLeafContainingPointFast(exitVolume->bspc.nodes, 0x7FFFFFFF, u.q) != 0;
        switch (exitVolume->state)
        {
        case 3:
            if (isInside)
                TakeExit(exitVolume);
            break;
        case 1:
            if (isInside)
                SetExitExits(exitVolume, 0);
            else if (g_clock.time - exitVolume->stateTime > exitVolume->rearmDelay)
                SetExitExits(exitVolume, 3);
            break;
        case 0:
            if (!isInside)
                SetExitExits(exitVolume, 1);
            break;
        }
        break;
    }
    case 2:
        break;
    }
}

// Loads a WARP from the BRX: the XFM part, then skips a string.
// Layout parse-verified: a parser following it reads every byte of 4 levels.
void WARP::LoadFromBrx(CBinaryInputStream *stream)
{
    WARP *warp = this;
    LoadXfmFromBrx(warp, stream);
    CBinaryInputStream_SkipStringSw(stream);
}

// Triggers a warp on arrival: moves each paired object to its target, triggers the warp's rsmgs, applies its task
// states, re-arms every exit that is not off, cuts the camera and sends the Splice trigger event (5). (Sly 1: TriggerWarp.)
// Also known as: ApplyCheckpoint (upstream readable / Sly 1 name; ApplyCheckpoint__FP2SW).
extern "C" void TriggerWarp(WARP *warp)
{
    EXIT *exitList[32];
    int i, j;
    for (i = 0; i < (int)warp->pairCount; i++)
    {
        WARPPAIR *pair = &warp->pairs[i];
        LO *object = FindSwObject(&g_sw, 4, pair->object, (LO *)warp);
        if (object)
        {
            LO *target = FindSwObject(&g_sw, 4, pair->target, (LO *)warp);
            if (target)
            {
                AsLo(object)->MatchTransform(target);
            }
        }
    }
    TriggerRsmg(&g_sw, warp->rsmgCount, warp->rsmgs, warp, 1);
    ApplyTaskStates(&g_game, warp->taskStateCount, warp->taskStates, warp);
    for (j = FindSwObjectsByClass(&g_sw, 5, 0x1A, 0, 0x20, exitList) - 1; j >= 0; j--)
    {
        if (exitList[j]->state != 2)
            SetExitExits(exitList[j], 0);
    }
    CutCm(&g_cm);
    warp->HandleSpliceEvent(5, 0, 0, 0, 0);
}

// Adds a rsmg (a message sent when the warp triggers) to the warp's four-entry list when the trigger kind is set.
// Sly 1: SetWarpRsmg.
extern "C" void SetWarpRsmg(WARP *warp, int d, OID e, OID f, OID g)
{
    if (d)
        AddOrUpdateRsmg(&g_sw, warp->rsmgs, 4, &warp->rsmgCount, d, e, f, g);
}

// Adds a task-state entry to the warp's four-entry list (applied when the warp triggers).
extern "C" void AddWarpTaskState(WARP *warp, int d, int e)
{
    AppendBoundedPair(&g_game, warp->taskStates, 4, &warp->taskStateCount, d, e);
}

// Adds an (object, target) pair to the warp: on trigger the object is moved to the target. At most four pairs.
// Also known as: MatchStepguardAnimationPhase (upstream readable / Sly 1 name;
//   MatchStepguardAnimationPhase__FP9STEPGUARD3OIDN31).
extern "C" void AddWarpOidPair(WARP *warp, OID objectOid, OID targetOid)
{
    unsigned int c = warp->pairCount;
    if (c < 4)
    {
        WARPPAIR *pair;
        warp->pairCount = c + 1;
        pair = &warp->pairs[c];
        pair->object = objectOid;
        pair->target = targetOid;
    }
}

// Adds an alias oid the warp also answers to in FindWarpByOid. At most four.
extern "C" void AddWarpOidAlias(WARP *warp, OID oid)
{
    if ((unsigned int)warp->aliasCount < 4)
        warp->aliases[warp->aliasCount++] = oid;
}

// Makes the warp answer to oid 0x189, the default start warp (the oid EXITs target by default). (inferred)
extern "C" void AddWarpAliasStart(WARP *warp)
{
    AddWarpOidAlias(warp, (OID)0x189);
}

// Makes the warp answer to oid 0x18A, the second well-known warp oid (inferred: the "end" warp).
extern "C" void AddWarpAliasEnd(WARP *warp)
{
    AddWarpOidAlias(warp, (OID)0x18A);
}
