/* include/sly2/swalloc.h -- SWALLOC, a world-heap / stack allocation request (byte count, alignment), passed by pointer
 * to the allocators (AllocClearedFromWorldHeap, AllocFromStack...; Sly 1's PvAllocSwImpl family). The decompiled TUs
 * had copied it as MSG, AL, REQ, REQUEST, AREQ (emitter.h) and ALLOCREQ (dmas.h). Self-contained, so that dmas.h, which
 * does not include sly2/basic.h, shares the one definition. */
#ifndef SLY2_SWALLOC_H
#define SLY2_SWALLOC_H
struct SWALLOC { int cb; int cbAlign; };
#endif
