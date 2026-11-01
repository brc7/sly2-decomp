// RefGetPeopid: ported from Lilly's personal Sly 2 decomp (src/P2/spliceobj.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: D_00452170 (g_slotheapMethod)
// 0x001FFD98 RefFromPv__FPvP5PVREC  (MATCHES Sly 2, T0997; our name)
// The splice value of an option. grf bit 1: a method - a CMethod (object, thunk, argument count) in the CRef. Bit 2: the member is read directly
// at pv + ib; else its getter (a pointer to member function, called with the object: pv itself or, with grf bit 0x2000, what pmfObj(1) returns).
// Then by type: 0 bool (!= 0), 1 float, 2 vector4 (by pointer / by value), 4 matrix3 (into a matrix CRef: rows copied, the w column cleared),
// 5..8 14..17 19 20 23 (and 13 for getters) int, 9 clq, 10 lm, 11 smp (the getters fill the CRef's block: SetTag, then call with the block; type 10 uses
// tag 0x1C like 9, as the game does), and by the flag bits of the type: 0x8000 the getter returns a CRef, 0x1000 int, 0x2000 BASIC *. The case order is
// the code's order: the int group first, then 1, 0, 2, 4, 9, 10, 11 (the layout of the arms follows the source order).
// How it matches (decomp-compiler-quirks.md section 29): (1) the record's getter is a UNION of typed pointers to member functions, called without a cast -
// `(pobj->*ppvrec->pmfGetM)()`. A cast `(MATRIX3 *(BASIC::*)())ppvrec->pmfGet` gives the same code except two instruction pairs in the arms whose
// delay slot gets a hoisted load (99.2%); (2) `CRef ref;` is a plain store of -1 in the tag (ref.h: CRef() no longer calls SetTag; spaprops.c builds its empty CRef with
// CRef((TAGK)0x24)); (3) `CMethod *pmethodNew = PmethodNew(); pmethod.Attach(pmethodNew);` as two statements; (4) the matrix `MATRIX4 *pmat = ref.m_tag.m_pmatrix;` once.
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
    void Unlink();
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
class CRef;
extern SLOTHEAP D_00452170;
class CMethod : public CGcObj<CMethod>
{
public:
      BASIC *m_pbasic;
      CRef (*m_pfnthunk)(BASIC *, int, CRef *);
      int m_crefReq;
    void operator delete(void *pv) { FreeSlotheapPv(&D_00452170, pv); }
};
CMethod* PmethodNew();
enum BIFK
{
};
template <class T>
class CRefPtr
{
public:
    T *m_p;
    CRefPtr() {}
    void Attach(T *p);
    CRefPtr(T *p) { Attach(p); }
    CRefPtr(const CRefPtr &ref)
    {
        m_p = ref.m_p;
        if (m_p)
        {
            m_p->m_cref++;
        }
    }
    ~CRefPtr()
    {
        if (m_p)
        {
            m_p->Release();
        }
    }
    void operator=(T *p);
    void operator=(const CRefPtr &ref) { *this = ref.m_p; }
    operator T *() { return m_p; }
    T *operator->() { return m_p; }
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
    void SetS32(int n);
    void SetF32(float g);
    void SetBool(int fBool);
    void SetVector4(const VECTOR4 *pvec);
    void SetVector4(qword qw);
    void SetClq(CLQ *pclq);
    void SetLm(LM *plm);
    void SetSmp(SMP *psmp);
    CRef *SetBasic(BASIC *pbasic);
    void SetMethod(CMethod *pmethod);
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
        long (BASIC::*pmfGetB)();
        float (BASIC::*pmfGetF)();
        qword (BASIC::*pmfGetQ)();
        MATRIX3 *(BASIC::*pmfGetM)();
        int (BASIC::*pmfGetN)();
        void (BASIC::*pmfGetClq)(CLQ *);
        void (BASIC::*pmfGetLm)(LM *);
        void (BASIC::*pmfGetSmp)(SMP *);
        CRef (BASIC::*pmfGetRef)();
        BASIC *(BASIC::*pmfGetPo)();
    };
      union
    {
        int ibSet;
        PMF pmfSet;
        PFNTHUNK pfnthunk;
    };
      union { SETTER setFile; int ibFile; int crefReq; };
      void *(BASIC::*pmfObj)(int);
};
struct PVREC : EOPID
{
};
CRef RefFromPv(void *pv, PVREC *ppvrec) __asm__("RefGetPeopid");
CRef RefFromPv(void *pv, PVREC *ppvrec)
{
    CRef ref;
    uint grf = ppvrec->grfeopid;
    if (grf & 1)
    {
        CRefPtr<CMethod> pmethod;
        CMethod *pmethodNew = PmethodNew();
        pmethod.Attach(pmethodNew);
        pmethod->m_pbasic = (BASIC *)pv;
        pmethod->m_pfnthunk = ppvrec->pfnthunk;
        pmethod->m_crefReq = ppvrec->crefReq;
        ref.SetMethod(pmethod);
    }
    else if (grf & 2)
    {
        if (grf & 0x2000)
        {
            pv = (((BASIC *)pv)->*ppvrec->pmfObj)(1);
        }
        void *pf = (char *)pv + ppvrec->ib;
        switch (ppvrec->otyp)
        {
        case 5: case 6: case 7: case 8:
        case 14: case 15: case 16: case 17: case 19: case 20: case 23:
            ref.SetS32(*(int *)pf);
            break;
        case 1:
            ref.SetF32(*(float *)pf);
            break;
        case 0:
            ref.SetBool(*(int *)pf != 0);
            break;
        case 2:
            ref.SetVector4((VECTOR4 *)pf);
            break;
        case 4:
        {
            ref.Decref();
            ref.SetTag((TAGK)0x1b);
            MATRIX4 *pmat = ref.m_tag.m_pmatrix;
            *(MATRIX3 *)pmat = *(MATRIX3 *)pf;
            pmat->mat[0][3] = 0.0f;
            pmat->mat[1][3] = 0.0f;
            pmat->mat[2][3] = 0.0f;
            break;
        }
        case 9:
            ref.SetClq((CLQ *)pf);
            break;
        case 10:
            ref.SetLm((LM *)pf);
            break;
        case 11:
            ref.SetSmp((SMP *)pf);
            break;
        case 13:
            break;
        default:
            if (ppvrec->otyp & 0x1000)
            {
                ref.SetS32(*(int *)pf);
            }
            else if (ppvrec->otyp & 0x2000)
            {
                ref.SetBasic(*(BASIC **)pf);
            }
            break;
        }
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
            ref.SetS32((pobj->*ppvrec->pmfGetN)());
            break;
        case 1:
            ref.SetF32((pobj->*ppvrec->pmfGetF)());
            break;
        case 0:
            ref.SetBool((pobj->*ppvrec->pmfGetB)() != 0);
            break;
        case 2:
            ref.SetVector4((pobj->*ppvrec->pmfGetQ)());
            break;
        case 4:
        {
            ref.Decref();
            ref.SetTag((TAGK)0x1b);
            MATRIX4 *pmat = ref.m_tag.m_pmatrix;
            MATRIX3 *pmatGot = (pobj->*ppvrec->pmfGetM)();
            *(MATRIX3 *)pmat = *pmatGot;
            pmat->mat[0][3] = 0.0f;
            pmat->mat[1][3] = 0.0f;
            pmat->mat[2][3] = 0.0f;
            break;
        }
        case 9:
            ref.Decref();
            ref.SetTag((TAGK)0x1c);
            (pobj->*ppvrec->pmfGetClq)(ref.m_tag.m_pclq);
            break;
        case 10:
            ref.Decref();
            ref.SetTag((TAGK)0x1c);
            (pobj->*ppvrec->pmfGetLm)(ref.m_tag.m_plm);
            break;
        case 11:
            ref.Decref();
            ref.SetTag((TAGK)0x1e);
            (pobj->*ppvrec->pmfGetSmp)(ref.m_tag.m_psmp);
            break;
        default:
            if (ppvrec->otyp & 0x8000)
            {
                CRef refGot = (pobj->*ppvrec->pmfGetRef)();
                ref = refGot;
            }
            else if (ppvrec->otyp & 0x1000)
            {
                ref.SetS32((pobj->*ppvrec->pmfGetN)());
            }
            else if (ppvrec->otyp & 0x2000)
            {
                ref.SetBasic((pobj->*ppvrec->pmfGetPo)());
            }
            break;
        }
    }
    return ref;
}
