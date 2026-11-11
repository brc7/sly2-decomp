// callback.cpp -- SW callbacks: (object, member function, argument) records queued on the world and run once by
// ProcessSwCallbacks, which also fires the due Splice event callbacks. TU 0x149A50..0x14A008 (6 functions, address
// order).
// Entry points: PostSwCallback / EnsureSwCallback (queue), ProcessSwCallbacks (once per frame).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_callback
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_callback
#include "sly2/pair.h"
#include "sly2/cframe.h"
#include "sly2/sp.h"
#include "sly2/clock.h"
#include "sly2/basic.h"
#include "sly2/sw.h"

// 0x149EB0 StaticInit_callback and GlobalConstructors_callback: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

/* The callback record and its pointer-to-member method: sly2/sw.h SWCALLBACK (CObj, PMFCB). */

typedef CRefOwner<CFrame> SPF;   /* owning pointer to a frame (inline release; ~CFrame is the linkonce 0x257078) */
struct SPFV { CFrame *p; };

/* A due Splice event callback. */
struct EVCB
{
    float time;
    SPFV *frameSlot;
    CPair *call;
    int id;
};
typedef char assert_EVCB_prefix[sizeof(EVCB) + sizeof(SWEVENT *) == sizeof(SWEVENT) ? 1 : -1];   /* sly2/sw.h SWEVENT without its link */



extern "C" {
void AppendSlEntry(SL *list, SWCALLBACK *callback);
void ClearSl(SL *list);
int IsSlEmpty(SL *list);
void RemoveSlEntry(SL *list, void *pv);
void EvalSpliceEventCallback(void *pv, CPair *ppair, CFrame *pframe, int n);
void PostSwCallback(SW *world, CObj *object, PMFCB method, int arg);
}

// Queues a deferred callback {object, member function, argument} on the world (node from the world's pool); it runs
// once in ProcessSwCallbacks.
// Also known as: PostDeferredPointerCall (upstream readable / Sly 1 name;
//   PostDeferredPointerCall__FPvUi22PFNDEFERREDPOINTERCALLi).
extern "C" void PostSwCallback(SW *world, CObj *object, PMFCB method, int arg)
{
    SWCALLBACK *callback = (SWCALLBACK *)AllocZeroedFromSlotheap(&world->slotheapCallback);
    callback->object = object;
    callback->method = method;
    callback->argument = arg;
    AppendSlEntry(&world->slCallback, callback);
}

// Queues the callback unless an identical {object, member function, argument} is already queued.
// Also known as: PostDeferredCall (upstream readable / Sly 1 name; PostDeferredCall__FPvUi15PFNDEFERREDCALLi).
extern "C" void EnsureSwCallback(SW *world, CObj *object, PMFCB method, int arg)
{
    SWCALLBACK *callback;
    for (callback = (SWCALLBACK *)world->slCallback.head; callback; callback = callback->next)
    {
        if (callback->object == object && callback->method == method && callback->argument == arg)
            return;
    }
    PostSwCallback(world, object, method, arg);
}

// Removes the queued callbacks that match the selected fields (flag 1 object, 2 member function, 4 argument).
// Also known as: CancelDeferredCalls (upstream readable / Sly 1 name; CancelDeferredCalls__FPvUiiii).
extern "C" void ClearSwCallbacks(SW *world, unsigned int matchFlags, CObj *object, PMFCB method, int arg)
{
    SWCALLBACK **link = (SWCALLBACK **)&world->slCallback.head;
    SWCALLBACK *callback;
    while ((callback = *link) != 0)
    {
        if ((matchFlags & 1) && object != callback->object)
            goto next;
        if ((matchFlags & 2) && !(method == callback->method))
            goto next;
        if ((matchFlags & 4) && arg != callback->argument)
            goto next;
        *link = callback->next;
        FreeSlotheapSlot(&world->slotheapCallback, callback);
        continue;
    next:
        link = &callback->next;
    }
    if (!world->slCallback.head)
        world->slCallback.tail = 0;
}

// Runs the queued callbacks (again while running them queues new ones), then every scheduled Splice callback whose time
// has come (EvalSpliceEventCallback), freeing each. Called once per frame by the world update.
// Also known as: RunDeferredSpliceEvents (upstream readable / Sly 1 name; RunDeferredSpliceEvents__FP2SW).
extern "C" void ProcessSwCallbacks(SW *world)
{
    while (!IsSlEmpty(&world->slCallback))
    {
        SWCALLBACK *callback = (SWCALLBACK *)world->slCallback.head;
        ClearSl(&world->slCallback);
        while (callback)
        {
            PMFCB method = callback->method;
            (callback->object->*method)(callback->argument);
            SWCALLBACK *next = callback->next;
            FreeSlotheapSlot(&world->slotheapCallback, callback);
            callback = next;
        }
    }

    EVCB *spliceCallback;
    while ((spliceCallback = (EVCB *)world->slEvent.head) != 0)
    {
        if (g_clock.time < spliceCallback->time)
            break;
        spliceCallback->id = -1;
        SPFV *frameSlot = spliceCallback->frameSlot;
        if (frameSlot)
        {
            EvalSpliceEventCallback(frameSlot, spliceCallback->call, frameSlot->p, 0);
        }
        else
        {
            SPF frame(CGcObj<CFrame>::PtNew());
            EvalSpliceEventCallback(world, spliceCallback->call, frame.p, 0);
        }
        RemoveSlEntry(&world->slEvent, spliceCallback);
        CPair *ppair = spliceCallback->call;
        if (--ppair->m_cref == 0)
            delete ppair;
        FreeSlotheapSlot(&world->slotheapEvent, spliceCallback);
    }
}
