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
struct CMethod;


/* CRef: a Splice value: a type tag and a one-word payload (a number, or a counted pointer to a heap value). */
struct CRef
{
      int m_tagk;   /* the value's type (TAGK) */
      union
      {
            int n;   /* 0x15 s32, 0x17 bool, 0x18 symid, 0x19 bifk */
            float g;   /* 0x16 f32 */
            void *pv;   /* any pointer payload */
            /* the payload by tag (spliceobj reads and writes it typed; one word in every case) */
            VECTOR *pvec;           /* 0x1A */
            MATRIX4 *pmat;          /* 0x1B */
            CLQ *pclq;              /* 0x1C */
            LM *plm;                /* 0x1D */
            SMP *psmp;              /* 0x1E */
            CPair *ppair;           /* 0x1F */
            CProc *pproc;           /* 0x20 */
            BASIC *pbasic;          /* 0x21 */
            CMethod *pmethod;       /* 0x22 */
      } u;

      CRef() { m_tagk = -1; }
      CRef(int tagk) { InitTag(tagk); }
      CRef(const CRef &r);
      CRef(int n, int tagk) { m_tagk = tagk; u.n = n; }            /* a word with its tag, no retain (bif's builtins) */
      CRef(CProc *pproc) __asm__("CRef_ctor_CProc");                 /* a procedure value (eval: EvalLambda) */
      CRef(CPair *ppair) __asm__("CRef_ctor_CPair");
      CRef(const VECTOR *pvec) __asm__("CRef_ctor_VECTOR");           /* the payload types own a heap each (CSpliceHeap, sly2/slotheap.h) */
      CRef(const MATRIX4 *pmat) __asm__("CRef_ctor_MATRIX4");
      CRef(const CLQ *pclq) __asm__("CRef_ctor_CLQ");
      CRef(const LM *plm) __asm__("CRef_ctor_LM");
      CRef(const SMP *psmp) __asm__("CRef_ctor_SMP");
      CRef(BASIC *pbasic) __asm__("CRef_ctor_BASIC");
      ~CRef() { Decref(); }
      CRef &operator=(const CRef &r);
      CRef &operator=(bool f) __asm__("CRef_SetBool");            /* a boolean (tag 0x17) */
      CRef &operator=(CPair *ppair) __asm__("CRef_SetPair");      /* a list (tag 0x1F) */
      /* the other setters of splice/ref.cpp, as named members (operator= overloads would change how includers that
         assign through the converting constructors compile) */
      CRef &SetVector(const VECTOR *pvec) __asm__("CRef_SetVector"); /* tag 0x1A */
      CRef &SetProc(CProc *pproc) __asm__("CRef_SetProc");           /* tag 0x20 */
      CRef &SetBasic(BASIC *pbasic) __asm__("CRef_SetBasic");        /* tag 0x21 */
      bool operator==(const CRef &ref);
      void Incref();
      void Decref();
      CRef &SetS32(int n);
      CRef &SetF32(float g);
      void SetTag(TAGK tagk);
      void InitTag(int tagk) __asm__("SetTag__4CRef4TAGK");
      void SetNil() { Decref(); InitTag(0); }
      void Retag(int tagk) { Decref(); InitTag(tagk); }               /* release the payload, then set a new tag */
};
typedef char assert_CRef_size[sizeof(CRef) == 8 ? 1 : -1];

#endif
