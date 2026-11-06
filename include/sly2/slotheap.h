/* include/sly2/slotheap.h -- fixed-size slot allocator (TU slotheap, src/game/platform/memory/slotheap.cpp,
 * 0x11CCA8..0x11CF60) and the engine-wide slot pools that every game TU instantiates. */
#ifndef SLY2_SLOTHEAP_H
#define SLY2_SLOTHEAP_H
#include "common.h"   /* byte; deliberately not sly2/basic.h, so every TU can include this header without
                         pulling in (and colliding with) the shared type names */

struct SLOT;

/* SLOT (a free slot: its first word links to the next free slot) stays forward-declared: several unmerged files
 * still define it privately. Define it here once those files are merged. */

/* Layout CONFIRMED by match of every slotheap function (offsets 0x0..0x14); moved here unchanged from sw.h. */
/* SLOTHEAP: a pool of fixed-size slots carved from one block, with a free list (records of one kind: contacts,
   animation applications, Splice nodes...). */
struct SLOTHEAP
{
      int cb;              /* 0x00 slot size */
      int c;               /* 0x04 slots per block */
      int cbAlign;         /* 0x08 block alignment (passed to the world allocator) */
      SLOT *pslotFree;     /* 0x0C free list */
      int cAlloc;          /* 0x10 live slots */
      byte *ab;            /* 0x14 the block carved into slots (allocated on first use) */
};
typedef char assert_SLOTHEAP_size[sizeof(SLOTHEAP) == 0x18 ? 1 : -1];

#ifdef __cplusplus
/* The slotheap functions keep their current (unmangled) linker symbols until tools/rename.py gives them their final
 * names; the proposed names are in the comments. */
extern "C" {
SLOTHEAP *InitSlotheap(SLOTHEAP *pslotheap, int cb, int c, int cbAlign);   /* InitSlotheap */
void *PvAllocSlotheapClear(SLOTHEAP *pslotheap);
void FreeSlotheapSlot(SLOTHEAP *pslotheap, void *pv);                          /* FreeSlotheapPv */
void ResetSlotheap(SLOTHEAP *pslotheap);
void PrimeSlotheap(SLOTHEAP *pslotheap);                                    /* PrimeSlotheap */
}

/* Engine-wide slot pools. Every game TU includes this header and so instantiates the six pools; g++ 2.95 then emits
 * the per-TU __static_initialization_and_destruction_0 (named StaticInit_<tu> in config, 308 bytes) that runs
 * `if (++sentry == 1) InitSlotheap(&pool, cb, c, 16)` for each pool, and its _GLOBAL_$I$<first global> stub that
 * calls it with (1, 0xFFFF). Bind both with SLY2_ALIAS lines in the TU (see docs/guides/tu-merging.md).
 * CONFIRMED by match (byte-identical StaticInit and stub in every merged TU).
 * HYPOTHESIS: the original spelling (template shape, pooled types, names). The pool objects are 0x28 apart in .bss
 * (0x4520A8..0x452170), so the real wrapper is larger than SLOTHEAP; data layout does not affect the code match. */
/* CSlotPool<CB, C>: one engine-wide pool of C slots of CB bytes (a static SLOTHEAP). */
template <int CB, int C>
struct CSlotPool
{
      enum { cb = CB, c = C };   /* slot size, slots per block */
      SLOTHEAP slotheap;
      CSlotPool() { InitSlotheap(&slotheap, CB, C, 16); }
      static CSlotPool s_pool;
};
template <int CB, int C> CSlotPool<CB, C> CSlotPool<CB, C>::s_pool;

/* One line per game TU, after its functions: instantiates the six pools in the order of the original static init. */
#define SLY2_INSTANTIATE_SLOT_POOLS() \
      template CSlotPool<0x14, 0x2000> CSlotPool<0x14, 0x2000>::s_pool; \
      template CSlotPool<0x18, 0x800> CSlotPool<0x18, 0x800>::s_pool; \
      template CSlotPool<0x28, 0x200> CSlotPool<0x28, 0x200>::s_pool; \
      template CSlotPool<0x1C, 0x800> CSlotPool<0x1C, 0x800>::s_pool; \
      template CSlotPool<0xC, 0xC8> CSlotPool<0xC, 0xC8>::s_pool; \
      template CSlotPool<0x14, 0x100> CSlotPool<0x14, 0x100>::s_pool;
/* Five more slot heaps, for the payloads of Splice values (CRef tags 0x1A..0x1E, splice/ref.cpp): vectors (0x20 x
   0x100), matrices (0x50 x 0x100), clqs (0x20 x 0x40), lms (0xC x 0x40) and smps (0x10 x 0x20), each payload followed
   by its reference count. Template statics like the pools: a TU that names them instantiates all five right after
   SLY2_INSTANTIATE_SLOT_POOLS(), and its static initializer then grows from 77 to 133 instructions (CONFIRMED by
   match: spliceutils, ref; HYPOTHESIS: the template shape and name). Their objects are 0x18 apart
   (0x452188..0x4521E8), i.e. a bare SLOTHEAP. */
/* CSpliceHeap<CB, C>: the slot heap of one Splice payload type. */
template <int CB, int C>
struct CSpliceHeap
{
      SLOTHEAP slotheap;
      CSpliceHeap() { InitSlotheap(&slotheap, CB, C, 16); }
      static CSpliceHeap s_heap;
};
template <int CB, int C> CSpliceHeap<CB, C> CSpliceHeap<CB, C>::s_heap;

#define SLY2_INSTANTIATE_SPLICE_HEAPS() \
      template CSpliceHeap<0x20, 0x100> CSpliceHeap<0x20, 0x100>::s_heap; \
      template CSpliceHeap<0x50, 0x100> CSpliceHeap<0x50, 0x100>::s_heap; \
      template CSpliceHeap<0x20, 0x40> CSpliceHeap<0x20, 0x40>::s_heap; \
      template CSpliceHeap<0xC, 0x40> CSpliceHeap<0xC, 0x40>::s_heap; \
      template CSpliceHeap<0x10, 0x20> CSpliceHeap<0x10, 0x20>::s_heap;
#endif

#endif
