// func_00200598: ported from Lilly's personal Sly 2 decomp (src/P2/spliceobj.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: D_00452170 (g_slotheapMethod)
// 0x00200598 RefSetPeopid__FPvP5PVRECP4CRef  (MATCHES Sly 2, T1009; our name)
// The setter twin of RefFromPv: writes the splice value *pref into option *ppvrec of the object pv (script options 'Set'/'Add'; RefSetOption below).
// The result is the void reference (tag 0x24). grf bit 0x10: the member is written directly - RefSetPvsFromSplice (0x201110, converts the value to the
// record's type) stores into pv + the offset at +0x10 (pv is first replaced by pmfObj(1) with grf bit 0x2000, as in RefFromPv). Else the setter at +0x10
// (the same union of typed pointers to member functions, called without casts) gets the value by type: int group, float (tag 0x16, else RefCoerceF32),
// and for bool / vector4 (by value, *pvector) / matrix / clq / lm / smp / int-flag / BASIC *-flag the payload word of the reference WITHOUT a tag check
// or conversion. Match notes: the bool setter takes `int` - with `long` (a DImode argument) the arm no longer cross-jumps into the shared
// `lw a1,4(s1); jalr` tail with the others (394 instead of 386 instructions).
typedef unsigned char    uint8_t;
typedef unsigned int     uint32_t;
typedef uint8_t     uchar;
typedef uint32_t    uint;
typedef unsigned long long qword __attribute__((mode(TI)));
typedef uchar byte;
template <class T>
class CGcObj
{
public:
      int m_cref;
      uint m_grfGc;
    ~CGcObj() { Unlink(); }
    void Release()
    {
        if (--m_cref == 0)
        {
            delete (T *)this;
        }
    }
};
struct SLOT
{
      SLOT *pslotNext;
};
struct SLOTHEAP
{
      int cb;
      int c;
      int cbAlign;
      SLOT *pslotFree;
      int cAlloc;
      byte *ab;
};
void FreeSlotheapPv(SLOTHEAP *pslotheap, void *pv);
struct BASIC;
extern SLOTHEAP D_00452170;
class CMethod : public CGcObj<CMethod>
{
public:
      BASIC *m_pbasic;
      int m_crefReq;
    void operator delete(void *pv) { FreeSlotheapPv(&D_00452170, pv); }
};
enum BIFK
{
};
struct CLQ;
struct LM;
struct VECTOR
{
    float x, y, z;
};
struct VECTOR4
{
    float x, y, z, w;
} __attribute__((aligned(16)));
struct MATRIX4
{
	float mat[4][4];
} __attribute__((aligned(16)));
struct MATRIX3
{
	float mat[3][4];
} __attribute__((aligned(16)));
struct SMP;
class CPair;
class CProc;
enum TAGK
{
    TAGK_Nil = -1,
};
union TAG
{
    int m_n;
    float m_g;
    int m_bool;
    uint m_symid;
    BIFK m_bifk;
    const VECTOR4* m_pvec4;
    CPair* m_ppair;
    CProc* m_pproc;
    VECTOR* m_pvector;
    MATRIX4* m_pmatrix;
    CLQ* m_pclq;
    LM* m_plm;
    SMP* m_psmp;
    BASIC* m_pbasic;
    CMethod* m_pmethod;
};
class CRef
{
public:
    TAGK m_tagk;
    TAG m_tag;
    CRef() { m_tagk = TAGK_Nil; }
    CRef(TAGK tagk) { SetTag(tagk); }
    CRef(TAGK tagk, int n) { m_tagk = tagk; m_tag.m_n = n; }
    CRef(TAGK tagk, float g) { m_tagk = tagk; m_tag.m_g = g; }
    CRef(const CRef &ref);
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &ref);
    bool operator==(const CRef &ref);
    void Decref();
    void SetTag(TAGK tagk);
    CRef RefCoerceS32() const;
    CRef RefCoerceF32() const;
};
typedef int GRFEOPID;
enum OTYP
{
};
typedef void (BASIC::*PMF)();
typedef CRef (*PFNTHUNK)(BASIC *, int, CRef *);
union SETTER
{
    int ibSet;
    PMF pmfSet;
    PFNTHUNK pfnthunk;
};
struct EOPID
{
      OTYP otyp;
      GRFEOPID grfeopid;
      union
    {
        int ib;
        PFNTHUNK pfnthunkDeferred;
        PMF pmfGet;
    };
      union
    {
        int ibSet;
        PMF pmfSet;
        PFNTHUNK pfnthunk;
        void (BASIC::*pmfSetB)(int);
        void (BASIC::*pmfSetF)(float);
        void (BASIC::*pmfSetQ)(qword);
        void (BASIC::*pmfSetN)(int);
        void (BASIC::*pmfSetM)(MATRIX3 *);
        void (BASIC::*pmfSetClq)(CLQ *);
        void (BASIC::*pmfSetLm)(LM *);
        void (BASIC::*pmfSetSmp)(SMP *);
        void (BASIC::*pmfSetPo)(BASIC *);
    };
      union { SETTER setFile; int ibFile; int crefReq; };
      void *(BASIC::*pmfObj)(int);
};
CRef RefSetPvsFromSplice(int cref, CRef *aref, OTYP *aotyp, void **apv);
struct PVREC : EOPID
{
};
CRef RefSetPeopid(void *pv, PVREC *ppvrec, CRef *pref) __asm__("func_00200598");
/** @sly2-annot addr=0x200598
 * Purpose: Setter twin: write a Splice value into an object option
 * Info: proposed name RefSetPeopid (T1); TU spliceobj
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7_1F8DD8+verdicts_tu_r7r8
 * See: docs/engine/splice-scripting.md#7-key-functions,
 *   docs/reference/functions/splice-scripting.md#spliceobj-0x1ffcf8-0x201620
 * @end-sly2-annot */
CRef RefSetPeopid(void *pv, PVREC *ppvrec, CRef *pref)
{
    CRef ref;
    ref.Decref();
    ref.SetTag((TAGK)0x24);
    uint grf = ppvrec->grfeopid;
    if (grf & 0x10)
    {
        if (grf & 0x2000)
        {
            pv = (((BASIC *)pv)->*ppvrec->pmfObj)(1);
        }
        void *pvTarget = (char *)pv + ppvrec->ibSet;
        ref = RefSetPvsFromSplice(1, pref, (OTYP *)ppvrec, &pvTarget);
    }
    else
    {
        if (grf & 0x2000)
        {
            pv = (((BASIC *)pv)->*ppvrec->pmfObj)(1);
        }
        BASIC *pobj = (BASIC *)pv;
        switch (ppvrec->otyp)
        {
        case 5: case 6: case 7: case 8: case 13:
        case 14: case 15: case 16: case 17: case 19: case 20: case 23:
        {
            int n;
            if (pref->m_tagk == 0x15)
            {
                n = pref->m_tag.m_n;
            }
            else
            {
                CRef refInt;
                refInt = pref->RefCoerceS32();
                n = refInt.m_tag.m_n;
            }
            (pobj->*ppvrec->pmfSetN)(n);
            break;
        }
        case 1:
        {
            float g;
            if (pref->m_tagk == 0x16)
            {
                g = pref->m_tag.m_g;
            }
            else
            {
                CRef refF;
                refF = pref->RefCoerceF32();
                g = refF.m_tag.m_g;
            }
            (pobj->*ppvrec->pmfSetF)(g);
            break;
        }
        case 0:
            (pobj->*ppvrec->pmfSetB)(pref->m_tag.m_bool);
            break;
        case 2:
            (pobj->*ppvrec->pmfSetQ)(*(qword *)pref->m_tag.m_pvector);
            break;
        case 4:
            (pobj->*ppvrec->pmfSetM)((MATRIX3 *)pref->m_tag.m_pmatrix);
            break;
        case 9:
            (pobj->*ppvrec->pmfSetClq)(pref->m_tag.m_pclq);
            break;
        case 10:
            (pobj->*ppvrec->pmfSetLm)(pref->m_tag.m_plm);
            break;
        case 11:
            (pobj->*ppvrec->pmfSetSmp)(pref->m_tag.m_psmp);
            break;
        default:
            if (ppvrec->otyp & 0x1000)
            {
                (pobj->*ppvrec->pmfSetN)(pref->m_tag.m_n);
            }
            else if (ppvrec->otyp & 0x2000)
            {
                (pobj->*ppvrec->pmfSetPo)(pref->m_tag.m_pbasic);
            }
            break;
        }
    }
    return ref;
}
