typedef int qword __attribute__((mode(TI)));
typedef struct CRef { int m_tagk; int m_tag; } CRef;
CRef *CRef_ctor(CRef *pref, qword v);
CRef *func_001291A8(CRef *pref, CRef *prefSrc);
void Decref__4CRef(CRef *pref);
/** @sly2-annot addr=0x122B08
 * Purpose: Splice builtin "vector-project-normal": (a.b)b
 * Info: proposed name RefOpVectorProjectNormal (T1); TU bif
 * Confidence: high [T1 0.93] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef *RefOpVectorProjectNormal__FiP4CRefP6CFrame(CRef *ret, void *pbasic, CRef *aref)
{
    CRef ref;
    register qword a __asm__("vf1") = *(qword *)aref[0].m_tag;
    register qword b __asm__("vf2") = *(qword *)aref[1].m_tag;
    register qword t __asm__("vf3");
    __asm__ ("vmul.xyz %0, %0, %2\n"
        "vaddw.x %1, $vf0, $vf0w\n"
        "vadday.x ACC, %0, %0y\n"
        "vmaddz.x %0, %1, %0z\n"
        "vmulx.xyzw %1, %2, %0x" : "+j"(a), "=j"(t) : "j"(b));
    CRef_ctor(&ref, t);
    func_001291A8(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
