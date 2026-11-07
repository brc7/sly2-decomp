/* include/sly2/pair.h -- CPair, the Splice cons cell, and CPairPtr, the counted pointer that links pairs.
 *
 * A pair is a GC node (sly2/gc.h's CGcObj<CPair>: count, link word) with a value and a next link (0x14 bytes, the
 * first engine slot pool). The next link is a counted pointer: assigning it (0x256C58) takes a reference on the new
 * pair and releases the old one; destroying it (0x256D40) releases the pair, which destroys the pair's own link in
 * turn (the release is recursive down the list). Deleting a pair runs CGcObj's unlink and frees the slot into the pool.
 *
 * CPairPtr is a view of sly2/basic.h's CRefPtr<CPair> (CONFIRMED by match: splice/gc.cpp emits both functions as the
 * instances __as__t7CRefPtr1Z5CPairP5CPair and _$_t7CRefPtr1Z5CPair, linkonce, with the real template). The view
 * stays for its includers: they call both out of line, where the template's inline destructor would be inlined
 * unless instantiated in gc.cpp's order. It is an explicit specialization of the template, so both calls name the
 * instances' symbols (no asm label: g++ drops a labelled destructor's call; no alias line since data/vtables-2).
 * Callers that release a pair inline use sly2/sp.h's CRefOwner<CPair>. gc.cpp keeps its own view of the node (it
 * instantiates the real template).
 * See docs/guides/tu-merging.md, "Linkonce copies by family" and "Views unified". */
#ifndef SLY2_PAIR_H
#define SLY2_PAIR_H
#include "sly2/cref.h"
#include "sly2/slotheap.h"
#include "sly2/gc.h"

struct CPair;

/* counted link to a pair: the includers' view of CRefPtr<CPair> (sly2/basic.h), an explicit specialization
   with only what they use, so that its out-of-line destructor and assignment are the template's instances by their
   own symbols (_$_t7CRefPtr1Z5CPair, __as__t7CRefPtr1Z5CPairP5CPair: splice/gc.cpp emits them from the real template, which it does not see
   specialized). */
template <class T> class CRefPtr;
template <> class CRefPtr<CPair>
{
public:
      CPair *p;
      ~CRefPtr();
      void operator=(CPair *ppair);
};
typedef CRefPtr<CPair> CPairPtr;

/* CPair: a Splice cons cell (one list element). */
struct CPair : CGcObj<CPair>
{
      CRef ref;               /* 0x08 the element */
      CPairPtr next;          /* 0x10 the rest of the list */
      void SetNext(CPair *ppair) { next = ppair; }
};
/* emitted by splice/gc.cpp; declared so that includers call them and emit no copy */
template <> void CGcObj<CPair>::Unlink();
template <> CPair *CGcObj<CPair>::PtNew();
typedef char assert_CPair_size[sizeof(CPair) == 0x14 ? 1 : -1];

#endif
