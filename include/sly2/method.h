/* include/sly2/method.h -- CMethod, a Splice method value (tag 0x22): an engine object bound to one of its option
 * thunks, so that a script can call it like a procedure. Made by object/spliceobj.cpp (GetOptionRecordValue), applied
 * by splice/eval.cpp (EvalApply), released by splice/ref.cpp (CRef::Decref).
 *
 * A method is a GC node (sly2/gc.h's CGcObj<CMethod>: count, link word) of 0x14 bytes (the sixth engine slot pool).
 * splice/gc.cpp keeps its own view (it instantiates the pool members that this header declares as specializations). */
#ifndef SLY2_METHOD_H
#define SLY2_METHOD_H
#include "sly2/basic.h"
#include "sly2/gc.h"

class CMethod : public CGcObj<CMethod>
{
public:
      BASIC *m_pbasic;        /* 0x08 the object */
      PFNTHUNK m_pfnthunk;    /* 0x0C the option's thunk, called with the object and the arguments */
      int m_crefReq;          /* 0x10 the arguments it requires */
};
typedef char assert_CMethod_size[sizeof(CMethod) == 0x14 ? 1 : -1];   /* the method pool's slot size */
/* emitted by splice/gc.cpp; declared so that includers call them and emit no copy */
template <> CMethod *CGcObj<CMethod>::PtNew();
template <> void CGcObj<CMethod>::Unlink();

#endif /* SLY2_METHOD_H */
