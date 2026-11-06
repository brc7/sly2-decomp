/* include/sly2/pair.h -- CPair, the Splice cons cell, and CPairPtr, the counted pointer that links pairs.
 *
 * A pair is {count, GC link word, value, next} (0x14 bytes, the first engine slot pool). The next link is a counted
 * pointer: assigning it (0x256C58) takes a reference on the new pair and releases the old one; destroying it
 * (0x256D40) releases the pair, which destroys the pair's own link in turn (the release is recursive down the list).
 *
 * CPairPtr is a view of sly2/basic.h's CRefPtr<CPair> (CONFIRMED by match: splice/gc.cpp emits both functions as the
 * instances __as__t7CRefPtr1Z5CPairP5CPair and _$_t7CRefPtr1Z5CPair, linkonce, with the real template). The view
 * stays for its includers: they call both out of line, where the template's inline destructor would be inlined
 * unless instantiated in gc.cpp's order. The assignment is bound by asm label; the destructor cannot be (g++ drops
 * the call), so its includers reference _$_8CPairPtr, which config/link_aliases.txt binds to the instance. Callers
 * that release a pair inline (the "SPW"/"SPN" owners of CPair_CloneTo and the list builtins) keep their own views.
 * See docs/guides/tu-merging.md, "Linkonce copies by family" and "Views unified". */
#ifndef SLY2_PAIR_H
#define SLY2_PAIR_H
#include "sly2/cref.h"
#include "sly2/slotheap.h"

extern "C" void Unlink__t6CGcObj1Z5CPair(void *pv);

struct CPair;

/* counted link to a pair (out-of-line destructor and assignment) */
struct CPairPtr
{
      CPair *p;
      ~CPairPtr();
      void operator=(CPair *ppair) __asm__("__as__t7CRefPtr1Z5CPairP5CPair");
};

struct CPair
{
      int cref;               /* 0x00 reference count */
      int x4;                 /* 0x04 GC link word */
      CRef ref;               /* 0x08 the element */
      CPairPtr pnext;         /* 0x10 the rest of the list */
      void SetNext(CPair *ppair) { pnext = ppair; }
      /* Pairs live in the first engine slot pool (slot size 0x14 == sizeof(CPair)). */
      void operator delete(void *pv) { Unlink__t6CGcObj1Z5CPair(pv); func_0011CD98(&CSlotPool<0x14, 0x2000>::s_pool.slotheap, pv); }
};
typedef char assert_CPair_size[sizeof(CPair) == 0x14 ? 1 : -1];

#endif
