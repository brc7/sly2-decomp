/* include/sly2/proc.h -- CProc, a Splice procedure (closure): the frame it closes over, its parameter list and its
 * body (sly2/pair.h lists). Built by splice/eval.cpp (lambda), cloned by splice/proc.cpp.
 *
 * A proc is a GC node (sly2/gc.h's CGcObj<CProc>: count, link word) of 0x1C bytes (the fourth engine slot pool). Its
 * links release inline (sly2/sp.h's CRefOwner<T>): the destructor (0x257148, the implicit one in the original) inlines
 * all three releases (CONFIRMED by match). It is declared here and defined `inline` at the end of splice/ref.cpp, after
 * its callers there: they call it, and the definition is emitted as the linkonce copy; every other TU only calls it.
 * Assigning a link calls CRefPtr<T>'s assignment instance (out of line). splice/gc.cpp keeps its own view. */
#ifndef SLY2_PROC_H
#define SLY2_PROC_H
#include "sly2/pair.h"
#include "sly2/sp.h"

class CFrame;

struct CProc : CGcObj<CProc>
{
      CRefOwner<CFrame> m_pframe;     /* 0x08 the frame the procedure closes over */
      int m_cargMin;                  /* 0x0C required parameters */
      int m_fRest;                    /* 0x10 a rest parameter follows them */
      CRefOwner<CPair> m_ppairParams; /* 0x14 parameter list (symbols) */
      CRefOwner<CPair> m_ppairBody;   /* 0x18 body expressions */
      ~CProc();                       /* 0x257148 (splice/ref.cpp), see above */
};
typedef char assert_CProc_size[sizeof(CProc) == 0x1C ? 1 : -1];   /* the proc pool's slot size */
/* emitted by splice/gc.cpp; declared so that includers call them and emit no copy */
template <> void CGcObj<CProc>::Unlink();
template <> CProc *CGcObj<CProc>::PtNew();

#endif /* SLY2_PROC_H */
