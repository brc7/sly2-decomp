typedef int qword __attribute__((mode(TI)));
typedef struct CRef { int m_tagk; int m_tag; } CRef;
CRef *CRef_ctor(CRef *pref, qword v);
CRef *func_001291A8(CRef *pref, CRef *prefSrc);
void Decref__4CRef(CRef *pref);
/** @sly2-annot addr=0x122770
 * Purpose: Splice builtin "vector-cross-product": VU outer product (cross)
 * Info: proposed name RefOpVectorCrossProduct (T1); TU bif
 * Confidence: high [T1 0.93] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef *RefOpVectorCrossProduct__FiP4CRefP6CFrame(CRef *ret, void *pbasic, CRef *aref)
{
    CRef ref;
    register qword a __asm__("vf1") = *(qword *)aref[0].m_tag;
    register qword b __asm__("vf2") = *(qword *)aref[1].m_tag;
    register qword v __asm__("vf3");
    __asm__ ("vopmula.xyz ACC, %1, %2\n"
        "vopmsub.xyz %0, %2, %1" : "=j"(v) : "j"(a), "j"(b));
    CRef_ctor(&ref, v);
    func_001291A8(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
