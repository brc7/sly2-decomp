/* include/sly2/cref.h -- CRef, the Splice value reference (tag kind + payload), as the C++ splice TUs use it.
 * Layout: {int m_tagk; union payload} (8 bytes); tagk 0x15 int, 0x16 float, 0x17 bool, 0x24 void, -1 nil.
 * The inline members are part of the codegen contract: CRef() stores -1 and ~CRef() calls Decref() (CONFIRMED by
 * match in every merged TU that includes this header; it is the spelling 120+ per-function files already use).
 * C files and the generated Splice wrappers keep the plain-struct CRef of splicewrap.h; never include both. */
#ifndef SLY2_CREF_H
#define SLY2_CREF_H

struct CPair;
struct CProc;

struct CRef
{
      int m_tagk;
      union
      {
            int n;
            float g;
            void *pv;
      } u;

      CRef() { m_tagk = -1; }
      CRef(int tagk) { InitTag(tagk); }
      CRef(const CRef &r);
      CRef(CProc *pproc);                 /* a procedure value (eval: RefEvalLambda) */
      ~CRef() { Decref(); }
      CRef &operator=(const CRef &r);
      CRef &operator=(bool f);            /* a boolean (tag 0x17) */
      CRef &operator=(CPair *ppair);      /* a list (tag 0x1F) */
      void Decref();
      void InitTag(int tagk);
      void SetNil() { Decref(); InitTag(0); }
};
typedef char assert_CRef_size[sizeof(CRef) == 8 ? 1 : -1];

#endif
