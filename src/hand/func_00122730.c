typedef int qword __attribute__((mode(TI)));
typedef struct CRef { int m_tagk; int m_tag; } CRef;
static __inline__ int GDot(qword *pa, qword *pb)
{
    register qword a __asm__("vf1") = *pa;
    register qword b __asm__("vf2") = *pb;
    register qword t __asm__("vf3");
    int g;
    __asm__ ("vmul.xyz %0, %0, %2\n"
        "vaddw.x %1, $vf0, $vf0w\n"
        "vadday.x ACC, %0, %0y\n"
        "vmaddz.x %0, %1, %0z" : "+j"(a), "=j"(t) : "j"(b));
    __asm__ ("qmfc2.ni %0, %1" : "=r"(g) : "j"(a));
    return g;
}
/** @sly2-annot addr=0x122730
 * Purpose: Splice builtin "vector-dot-product": VU dot xyz
 * Info: proposed name RefOpVectorDotProduct (T1); TU bif
 * Notes: names_v2 C proposed RefOpVectorCrossProduct__FiP4CRefP6CFrame (0.167)
 * Confidence: high [T1 0.93] - evidence: static naming pass; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef *func_00122730(CRef *ret, void *pbasic, CRef *aref)
{
    int g = GDot((qword *)aref[0].m_tag, (qword *)aref[1].m_tag);
    ret->m_tagk = 0x16;
    *(volatile int *)&ret->m_tag = g;
    return ret;
}
