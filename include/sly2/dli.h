/* include/sly2/dli.h -- DLI, the safe iterator over a DL intrusive list, and its inline helpers.
 * Live iterators form a stack (g_pdliFirst, g_pdliFirst) so code that unlinks an element can fix up iterators that point
 * at it. Layout and helpers as the matched per-function files spell them (about 30 copies; Sly 1 names); CONFIRMED by
 * match in the merged TUs that include this header. A function pops its iterator by restoring
 * `g_pdliFirst = dli.m_pdliNext` at the end. */
#ifndef SLY2_DLI_H
#define SLY2_DLI_H
#include "sly2/basic.h"     /* DL */

/* DLI: an iterator over a DL that survives removal of the element it points at (live iterators are stacked in
   g_pdliFirst). */
struct DLI
{
      DL *m_pdl;   /* the list iterated */
      void **m_ppv;          /* the link field that holds the next element */
      int m_ibDle;           /* offset of the DLE inside an element */
      DLI *m_pdliNext;       /* next (older) live iterator */
};

extern DLI *g_pdliFirst;      /* g_pdliFirst */

// Start iterating display list pdl with iterator pdli and push it on the global iterator stack (g_pdliFirst), so
// removals during the walk can fix it up.
static inline void InitDli(DLI *pdli, DL *pdl)
{
      pdli->m_pdl = pdl;
      pdli->m_ppv = (void **)pdl;
      pdli->m_ibDle = pdl->linkOffset;
      pdli->m_pdliNext = g_pdliFirst;
      g_pdliFirst = pdli;
}

// Return the current element of the iterator and advance it along the element's link field.
static inline void *PvNextDli(DLI *pdli)
{
      void *pv = *pdli->m_ppv;
      pdli->m_ppv = (void **)((char *)pv + pdli->m_ibDle);
      return pv;
}

#endif
