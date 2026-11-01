typedef int qword __attribute__((mode(TI)));
typedef struct CRef { int m_tagk; int m_tag; } CRef;
static __inline__ int GLength(qword *pa)
{
    register qword a __asm__("vf1") = *pa;
    register qword t __asm__("vf2");
    int g;
    __asm__ ("vmul.xyz %0, %0, %0\n"
        "vaddw.x %1, $vf0, $vf0w\n"
        "vadday.x ACC, %0, %0y\n"
        "vmaddz.x %0, %1, %0z" : "+j"(a), "=j"(t));
    __asm__ ("vsqrt Q, %0x\n"
        "vwaitq\n"
        "vaddq.x %0, $vf0, Q" : "+j"(a));
    __asm__ ("qmfc2.ni %0, %1" : "=r"(g) : "j"(a));
    return g;
}
/** @sly2-annot addr=0x1227D0
 * Purpose: Splice builtin "vector-lth": VU length
 * Info: proposed name RefOpVectorLth (T1); TU bif
 * Confidence: high [T1 0.93] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef *func_001227D0(CRef *ret, void *pbasic, CRef *aref)
{
    int g = GLength((qword *)aref[0].m_tag);
    ret->m_tagk = 0x16;
    *(volatile int *)&ret->m_tag = g;
    return ret;
}
