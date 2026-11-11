// update.cpp -- the per-frame world update (UpdateSw and its object pass). TU 0x226550..0x2269B8 (4 functions,
// address order).
// Entry points: SW::Update (once per game frame, from the main loop) and UpdateSwObjects.
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_update
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_update
#include "sly2/slotheap.h"
#include "sly2/dli.h"
#include "sly2/sw.h"
#include "sly2/cm.h"
#include "sly2/aseg.h"
#include "sly2/lo.h"
#include "sly2/clock.h"
#include "sly2/save.h"
#include "sly2/sndmgr.h"
#include "sly2/po.h"

// 0x226960 StaticInit_update and GlobalConstructors_update: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

/* Objects are called through vtable slots 16/17 (explicit entries: {short delta; short pad; fn}). */

/* The fields of SW (sly2/sw.h, 0x4790 bytes) that the update touches; not yet named in sw.h. */

extern char g_gc[];
extern "C" {
void ProcessSwCallbacks(SW *p);
void UpdateSwBusyList(SW *p);
void UpdateSwObjects(SW *p);
void UpdatePoctl(POCTL *ppoctl);
void ExpireSwZapRecords(SW *p);
void UpdateSwGuardOverride(SW *p);
void UpdateAsega(void *pv);   /* takes an ASEGA* */
void ExpireNoise(void *pv);
void SolveSw(SW *p);
void UpdatePendingCid14Obstacles(void);
void UpdateShaders(void);
void UpdateSave__FP4SAVE(void *pv);
void UpdateSndmgr(void *pv);
void UpdateCmLast__FP2CM(void *pv);
void UpdateSwTimerHud(SW *p);
void CGc_UpdateRecyclable(void *pv);
}

/* The tagged LO flag word (lo.cpp SetLoFlags): a clear low bit means it points at a shared word. */
static __inline__ unsigned int GetLoFlags(LO *object) { unsigned int x = object->unknown40;   /* the tagged flag word */ return ((x ^ 1) & 1) ? object->ploupd->grf : x; }
/* Object entry flags tested here (inferred from the two passes): 0x2 updates every frame; 0x100 also while the game
   clock is stopped; 0x4 (without 0x20) gets the stopped-clock slot-17 call. */
enum { ENTRY_UPDATE = 0x2, ENTRY_PAUSED_CALL = 0x4, ENTRY_PAUSED_CALL_OFF = 0x20, ENTRY_UPDATE_WHEN_PAUSED = 0x100 };

// The per-frame world update (SW vtable slot 7): runs callbacks, updates the objects (UpdateSwObjects), lights, sound
// and the camera. Called once per game frame by the main loop.
// Also known as: RegisterSwPass (upstream readable / Sly 1 name; RegisterSwPass__FPv).
void SW::Update()
{
    SW *world = (SW *)this;
    ProcessSwCallbacks(world);
    UpdateSwBusyList(world);
    if (g_clock.deltaTime <= 0.0f) {
        UpdateSwObjects(world);
    } else {
        POCTL *lo;
        int i;
        DLI dli, dli2;
        void *element;
        for (lo = ((POCTL *)world->slPoctl.head); lo; lo = (POCTL *)lo->next)
            UpdatePoctl(lo);
        ExpireSwZapRecords(world);
        world->piloupdCur = &i;
        for (i = 0; i < world->cloupd; i++) {
            LOUPD *entry = &world->aloupd[i];
            if (entry->grf & ENTRY_UPDATE) {
                LO *object = entry->plo;
                object->Update();
            }
        }
        world->piloupdCur = 0;
        UpdateSwGuardOverride(world);
        InitDli(&dli, &world->dlAsega);
        while ((element = PvNextDli(&dli)) != 0)
            UpdateAsega(element);
        InitDli(&dli2, &world->dlStimulus);
        while ((element = PvNextDli(&dli2)) != 0)
            ExpireNoise(element);
        ProcessSwCallbacks(world);
        SolveSw(world);
        UpdatePendingCid14Obstacles();
        UpdateShaders();
        ProcessSwCallbacks(world);
        g_pdliFirst = dli.m_pdliNext;
    }
    UpdateSave__FP4SAVE(&g_save);
    UpdateSndmgr(&g_sndmgr);
    if (0.0f < g_clock.cameraDeltaTime)
        UpdateCmLast__FP2CM(&g_cm);
    UpdateSwTimerHud(world);
    CGc_UpdateRecyclable(g_gc);
}

// Walks the world's list of objects that want updates and calls each one's update (and its animation segments).
extern "C" void UpdateSwObjects(SW *world)
{
    DLI dli;
    int i;
    ASEGA *asega;
    world->piloupdCur = &i;
    for (i = 0; i < world->cloupd; i++) {
        LOUPD *entry = &world->aloupd[i];
        if ((entry->grf & (ENTRY_UPDATE_WHEN_PAUSED | ENTRY_UPDATE)) == (ENTRY_UPDATE_WHEN_PAUSED | ENTRY_UPDATE)) {
            LO *object = entry->plo;
            object->Update();
        }
    }
    world->piloupdCur = 0;
    InitDli(&dli, &world->dlAsega);
    while ((asega = (ASEGA *)PvNextDli(&dli)) != 0) {
        if (GetLoFlags(asega->animation) & ENTRY_UPDATE_WHEN_PAUSED)
            UpdateAsega(asega);
    }
    world->piloupdCur = &i;
    for (i = 0; i < world->cloupd; i++) {
        LOUPD *entry = &world->aloupd[i];
        if ((entry->grf & (ENTRY_UPDATE_WHEN_PAUSED | ENTRY_PAUSED_CALL_OFF | ENTRY_PAUSED_CALL)) == (ENTRY_UPDATE_WHEN_PAUSED | ENTRY_PAUSED_CALL)) {
            LO *object = entry->plo;
            object->UpdateXfWorld();   /* slot 17 */
        }
    }
    world->piloupdCur = 0;
    g_pdliFirst = dli.m_pdliNext;
}
