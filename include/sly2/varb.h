/* include/sly2/varb.h -- CVarb, a Splice variable binding, and CVarbPtr, the counted pointer that links bindings.
 *
 * A binding is {count, GC link word, symbol, value, next} (0x18 bytes, the second engine slot pool); frames and side
 * bags keep their variables as a list of bindings. The next link is a counted pointer: assigning it (0x257450) takes a
 * reference on the new binding and releases the old one; destroying it (0x257250, _$_8CVarbPtr) releases the binding,
 * which destroys the binding's own link in turn. Both are linkonce copies, homed in splice/frame.cpp and
 * splice/sidebag.cpp.
 *
 * Names: CVarbPtr is a placeholder, the binding-list twin of sly2/pair.h's CPairPtr (HYPOTHESIS: both are out-of-line
 * instances of one counted-pointer template, sly2/basic.h's CRefPtr<T>; see pair.h). Owners that release a binding
 * inline (the "SPW" views of CVarb_CloneTo and CSidebag) keep their own views.
 * See docs/guides/tu-merging.md, "Representation decisions" and "Views unified". */
#ifndef SLY2_VARB_H
#define SLY2_VARB_H
#include "sly2/cref.h"
#include "sly2/slotheap.h"

extern "C" void Unlink__t6CGcObj1Z5CVarb(void *pv);

struct CVarb;

/* counted link to a binding (out-of-line destructor and assignment) */
struct CVarbPtr
{
      CVarb *p;
      ~CVarbPtr();
      void operator=(CVarb *pvarb) __asm__("func_00257450");
};

struct CVarb
{
      int cref;               /* 0x00 reference count */
      int x4;                 /* 0x04 GC link word */
      int sym;                /* 0x08 the variable's symbol id */
      CRef ref;               /* 0x0C the value */
      CVarbPtr pnext;         /* 0x14 the next binding */
      void SetNext(CVarb *pvarb) { pnext = pvarb; }
      /* Bindings live in the second engine slot pool (slot size 0x18 == sizeof(CVarb)). */
      void operator delete(void *pv) { Unlink__t6CGcObj1Z5CVarb(pv); func_0011CD98(&CSlotPool<0x18, 0x800>::s_pool.slotheap, pv); }
};
typedef char assert_CVarb_size[sizeof(CVarb) == 0x18 ? 1 : -1];

#endif
