typedef struct CRef { int m_tagk; int m_tag; } CRef;
void func_00123EB0(CRef *ret, void *pbasic, int cref, CRef *aref, int k);
/** @sly2-annot addr=0x124018
 * Purpose: Splice builtin "seto!": op 0x6E RefSetOption
 * Info: proposed name RefOpSetO (T1); TU bif
 * Confidence: high [T1 0.93] - evidence: static analysis; sources: r1_116918+verdicts_A1
 * See: docs/engine/splice-scripting.md#7-key-functions,
 *   docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef *func_00124018(CRef *ret, void *pbasic, int cref, CRef *aref)
{
    func_00123EB0(ret, pbasic, cref, aref, 0x6E);
    return ret;
}
