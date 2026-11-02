/* include/sly2/dli.h -- DLI, the safe iterator over a DL intrusive list, and its inline helpers.
 * Live iterators form a stack (g_pdliFirst, D_002DD2FC) so code that unlinks an element can fix up iterators that point
 * at it. Layout and helpers as the matched per-function files spell them (about 30 copies; Sly 1 names); CONFIRMED by
 * match in the merged TUs that include this header. A function pops its iterator by restoring
 * `D_002DD2FC = dli.m_pdliNext` at the end. */
#ifndef SLY2_DLI_H
#define SLY2_DLI_H
#include "sly2/basic.h"     /* DL */

struct DLI
{
      DL *m_pdl;
      void **m_ppv;          /* the link field that holds the next element */
      int m_ibDle;           /* offset of the DLE inside an element */
      DLI *m_pdliNext;       /* next (older) live iterator */
};

extern DLI *D_002DD2FC;      /* g_pdliFirst */

static inline void InitDli(DLI *pdli, DL *pdl)
{
      pdli->m_pdl = pdl;
      pdli->m_ppv = (void **)pdl;
      pdli->m_ibDle = pdl->ibDle;
      pdli->m_pdliNext = D_002DD2FC;
      D_002DD2FC = pdli;
}

static inline void *PvNextDli(DLI *pdli)
{
      void *pv = *pdli->m_ppv;
      pdli->m_ppv = (void **)((char *)pv + pdli->m_ibDle);
      return pv;
}

#endif
