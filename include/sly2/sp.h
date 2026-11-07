/* include/sly2/sp.h -- CRefOwner<T>, the owning counted pointer of the splice/ TUs whose release is inline, and
 * CRefPtrFns<T>, the out-of-line CRefPtr<T> members of each GC node kind by symbol.
 *
 * In the original both are sly2/basic.h's CRefPtr<T>: one template whose destructor is inline. Where a TU inlined that
 * destructor (the owners the CloneTo functions return, the evaluator's locals) and also called it out of line (the
 * recursive release of a list's next link, inside the inlined node destructor), g++ 2.95 emitted a linkonce copy of
 * the destructor in that TU too; the linker kept gc.cpp's. Our objects must keep their symbol tables, so a TU that only
 * calls the copy sees CRefPtr<T>'s destructor declared out of line (the explicit specializations of sly2/pair.h and
 * varb.h, the member specialization of sly2/classes.h), and its inline-release locals use CRefOwner<T>, which is the
 * same object (one pointer) with the template's inline destructor: same code, no copy. The attach (CRefPtr<T>::Attach,
 * a linkonce instance emitted by splice/gc.cpp) is called by its symbol through CRefPtrFns<T>.
 * See docs/guides/tu-merging.md, "Views unified, continued (names/vu-f8-sound-ui-3)". */
#ifndef SLY2_SP_H
#define SLY2_SP_H
#include "sly2/gc.h"

/* The counted-pointer members of each kind that are called out of line, by the symbols of CRefPtr<T>'s instances
   (splice/gc.cpp; CMethod's Attach object/spliceobj.cpp). Static functions taking the pointer's address first: the
   same call as the member's. */
template <class T> struct CRefPtrFns;
#define SLY2_REFPTR_FNS(T, MANGLED) \
      template <> struct CRefPtrFns<T> \
      { \
            static void Attach(T **ppt, T *pt) __asm__("Attach__t7CRefPtr1Z" MANGLED "P" MANGLED); \
            static void Assign(T **ppt, T *pt) __asm__("__as__t7CRefPtr1Z" MANGLED "P" MANGLED); \
      };
SLY2_REFPTR_FNS(CPair, "5CPair")
SLY2_REFPTR_FNS(CVarb, "5CVarb")
SLY2_REFPTR_FNS(CFrame, "6CFrame")
SLY2_REFPTR_FNS(CProc, "5CProc")
SLY2_REFPTR_FNS(CSidebag, "8CSidebag")
#undef SLY2_REFPTR_FNS

/* CRefOwner<T>: a counted pointer to a GC node whose release is inline (a reference dropped to zero deletes the node
   here, running T's destructor). The default constructor leaves it unset (the CloneTo functions attach the new node
   right away); the copy constructor is declared only (returning one by value constructs it in place). */
template <class T>
struct CRefOwner
{
      T *p;
      CRefOwner() {}
      CRefOwner(T *pt) { Set(pt); }
      CRefOwner(const CRefOwner &);
      ~CRefOwner()
      {
            T *pt = p;
            if (pt && --pt->m_cref == 0)
                  delete pt;
      }
      void Set(T *pt) { CRefPtrFns<T>::Attach(&p, pt); }
      void operator=(T *pt) { CRefPtrFns<T>::Assign(&p, pt); }
      T *operator->() { return p; }
      operator T *() { return p; }
};

#endif /* SLY2_SP_H */
