/* include/sly2/slotheap.h -- fixed-size slot allocator (TU slotheap, 0x11CCA8..0x11CF60). */
#ifndef SLY2_SLOTHEAP_H
#define SLY2_SLOTHEAP_H
#include "dmas.h"          /* ALLOCREQ */

/* Layout CONFIRMED by match of every slotheap function (offsets 0x0..0x14). */
struct SLOTHEAP
{
    int cb;                /* 0x00 slot size */
    int c;                 /* 0x04 slots per block */
    int cbAlign;           /* 0x08 block alignment (passed to the world allocator) */
    void *pvFree;          /* 0x0C free list (first word of a free slot links to the next) */
    int cAlloc;            /* 0x10 live slots */
    void *pvBlock;         /* 0x14 the block carved into slots */
};

SLOTHEAP *InitSlotheap(SLOTHEAP *pslotheap, int cb, int c, int cbAlign);
void *PvAllocSlotheapClear(SLOTHEAP *pslotheap);
void FreeSlotheapPv(SLOTHEAP *pslotheap, void *pv);
void ResetSlotheap(SLOTHEAP *pslotheap);
void PrimeSlotheap(SLOTHEAP *pslotheap);

/* Engine-wide slot pools. Every game TU includes this header and so instantiates the six pools; g++ 2.95 then emits
 * the per-TU __static_initialization_and_destruction_0 (StaticInit_<tu>, 308 bytes) that runs
 * `if (++sentry == 1) InitSlotheap(&pool, cb, c, 16)` for each, plus its _GLOBAL_$I$ stub.
 * CONFIRMED by match: StaticInit_letterbox/_ms/_slotheap and their _GLOBAL_$I$ stubs compile byte-identical from this.
 * HYPOTHESIS: the original spelling (template shape, pooled types, names). The pool objects are 0x28 apart in .bss
 * (0x4520A8..0x452170), so the real wrapper is larger than SLOTHEAP; data layout does not affect the code match. */
template <int CB, int C>
struct CSlotPool
{
    SLOTHEAP slotheap;
    CSlotPool() { InitSlotheap(&slotheap, CB, C, 16); }
    static CSlotPool s_pool;
};
template <int CB, int C> CSlotPool<CB, C> CSlotPool<CB, C>::s_pool;

#define SLY2_INSTANTIATE_SLOT_POOLS() \
    template CSlotPool<0x14, 0x2000> CSlotPool<0x14, 0x2000>::s_pool; \
    template CSlotPool<0x18, 0x800> CSlotPool<0x18, 0x800>::s_pool; \
    template CSlotPool<0x28, 0x200> CSlotPool<0x28, 0x200>::s_pool; \
    template CSlotPool<0x1C, 0x800> CSlotPool<0x1C, 0x800>::s_pool; \
    template CSlotPool<0xC, 0xC8> CSlotPool<0xC, 0xC8>::s_pool; \
    template CSlotPool<0x14, 0x100> CSlotPool<0x14, 0x100>::s_pool;

#endif
