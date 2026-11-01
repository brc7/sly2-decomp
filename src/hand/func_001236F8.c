typedef struct CRef { int m_tagk; int m_tag; } CRef;
void func_00129518(CRef *pref, int n);
void func_00129558(CRef *pref, float g);
CRef *func_001291A8(CRef *pref, CRef *prefSrc);
void func_00129EC8(CRef *pref);
/** @sly2-annot addr=0x1236F8
 * Purpose: Splice builtin "abs": abs int/float
 * Info: proposed name RefOpAbs (T1); TU bif
 * Confidence: high [T1 0.93] - evidence: static analysis; sources: r1_116918+verdicts_A1
 * See: docs/engine/splice-scripting.md#7-key-functions,
 *   docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef *func_001236F8(CRef *ret, void *pbasic, CRef *aref)
{
    CRef ref;
    ref.m_tagk = -1;
    if (aref->m_tagk == 0x15) {
        int n = aref->m_tag;
        func_00129518(&ref, n < 0 ? -n : n);
    } else {
        float g = *(float *)&aref->m_tag;
        if (g < 0.0f) g = -g;
        func_00129558(&ref, g);
    }
    func_001291A8(ret, &ref);
    func_00129EC8(&ref);
    return ret;
}
