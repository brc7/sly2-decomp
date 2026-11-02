/* include/sly2/cref.h -- CRef, the Splice value reference (tag kind + payload), as the C++ splice TUs use it.
 * Layout: {int m_tagk; union payload} (8 bytes); tagk 0x15 int, 0x16 float, 0x17 bool, 0x24 void, -1 nil.
 * The inline members are part of the codegen contract: CRef() stores -1 and ~CRef() calls Decref() (CONFIRMED by
 * match in every merged TU that includes this header; it is the spelling 120+ per-function files already use).
 * C files and the generated Splice wrappers keep the plain-struct CRef of splicewrap.h; never include both. */
#ifndef SLY2_CREF_H
#define SLY2_CREF_H

/* The value tag. sly2/basic.h defines the same enum under the same guard, so either header may come first. Values
   (CONFIRMED by the constructors and setters of splice/ref.cpp, which store them): 0x15 s32, 0x16 f32, 0x17 bool,
   0x18 symid, 0x19 bifk, 0x1A vector, 0x1B matrix, 0x1C clq, 0x1D lm, 0x1E smp, 0x1F pair, 0x20 proc, 0x21 basic
   object, 0x22 method, 0x24 void. */
#ifndef SLY2_ENUM_TAGK
#define SLY2_ENUM_TAGK
enum TAGK { TAGK_Nil = -1 };
#endif

struct CPair;
struct CProc;
struct VECTOR;
struct MATRIX4;
struct CLQ;
struct LM;
struct SMP;
struct BASIC;


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
      CRef(CPair *ppair);
      CRef(const VECTOR *pvec);           /* the payload types own a heap each (CSpliceHeap, sly2/slotheap.h) */
      CRef(const MATRIX4 *pmat);
      CRef(const CLQ *pclq);
      CRef(const LM *plm);
      CRef(const SMP *psmp);
      CRef(BASIC *pbasic);
      ~CRef() { Decref(); }
      CRef &operator=(const CRef &r);
      CRef &operator=(bool f);            /* a boolean (tag 0x17) */
      CRef &operator=(CPair *ppair);      /* a list (tag 0x1F) */
      bool operator==(const CRef &ref);
      void Incref();
      void Decref();
      CRef &SetS32(int n);
      CRef &SetF32(float g);
      void SetTag(TAGK tagk);
      void InitTag(int tagk);
      void SetNil() { Decref(); InitTag(0); }
};
typedef char assert_CRef_size[sizeof(CRef) == 8 ? 1 : -1];

#endif
