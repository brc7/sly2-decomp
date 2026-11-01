typedef int qword __attribute__((mode(TI)));
struct CRef {
    int m_tagk; union { int n; float g; void *pv; } u;
    CRef() { m_tagk = -1; }
    CRef(int tagk) { InitTag(tagk); }
    CRef(const CRef &r);
    void Decref();
    void InitTag(int tagk);
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &r);
};
struct VECTOR { float x, y, z, w; } __attribute__((aligned(16)));
struct MATRIX3 { VECTOR mat[3]; };
struct W { char pad[0xC]; qword *pv; };
extern qword D_002A2DC0;
extern "C" {
MATRIX3 func_0011B830(qword a, qword b);
CRef func_00123208(void *pbasic, CRef *aref);
}
static inline qword VecSub(qword *pb, qword *pa)
{
    register qword a __asm__("vf2") = *pa;
    register qword b __asm__("vf1") = *pb;
    __asm__ ("vsub.xyzw %0, %0, %1" : "+j"(b) : "j"(a));
    return b;
}
/** @sly2-annot addr=0x123208
 * Purpose: Splice builtin "matrix-look-at": BuildOrthonormal from (to-from), up default D_002A2DC0
 * Info: proposed name RefOpMatrixLookAt (T1); TU bif
 * Confidence: high [T1 0.93] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef func_00123208(void *pbasic, CRef *aref)
{
    qword *pup;
    if (aref[2].m_tagk == 0)
        pup = &D_002A2DC0;
    else
        pup = ((W *)aref[2].u.pv)->pv;
    union { qword q; } v;
    v.q = VecSub((qword *)aref[1].u.pv, (qword *)aref[0].u.pv);
    CRef ref(0x1B);
    MATRIX3 *pmat = (MATRIX3 *)ref.u.pv;
    *pmat = func_0011B830(v.q, *pup);
    pmat->mat[2].w = 0.0f;
    pmat->mat[1].w = 0.0f;
    pmat->mat[0].w = 0.0f;
    return ref;
}
