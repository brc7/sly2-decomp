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
    CRef &operator=(float g);
};
struct W { char pad[0xC]; qword *pv; };
extern qword D_002A2DD0;
extern "C" {
CRef func_00122810(void *pbasic, CRef *aref);
}
static inline float SDist(qword *pa, qword *pb)
{
    register qword a __asm__("vf1") = *pa;
    register qword b __asm__("vf2") = *pb;
    register qword t __asm__("vf3");
    int n;
    float g;
    __asm__ ("vsub.xyzw %0, %0, %2\n"
        "vmul.xyz %0, %0, %0\n"
        "vaddw.x %1, $vf0, $vf0w\n"
        "vadday.x ACC, %0, %0y\n"
        "vmaddz.x %0, %1, %0z\n"
        "vsqrt Q, %0x\n"
        "vwaitq\n"
        "vaddq.x %0, $vf0, Q" : "+j"(a), "=j"(t) : "j"(b));
    __asm__ ("qmfc2.ni %0, %1" : "=r"(g) : "j"(a));
    return g;
}
/** @sly2-annot addr=0x122810
 * Purpose: Splice builtin "vector-distance": distance to a1 or origin
 * Info: proposed name RefOpVectorDistance (T1); TU bif
 * Confidence: high [T1 0.93] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef func_00122810(void *pbasic, CRef *aref)
{
    CRef ref;
    if (aref[1].m_tagk == 0)
        ref = SDist((qword *)aref[0].u.pv, &D_002A2DD0);
    else
    {
        register W *pw __asm__("$5") = (W *)aref[1].u.pv;
        ref = SDist((qword *)aref[0].u.pv, pw->pv);
    }
    return ref;
}
