typedef struct CRef { int m_tagk; int m_tag; } CRef;
void func_00120C48(CRef *ret, void *pbasic, int cref, CRef *aref, int k);
/** @sly2-annot addr=0x120CC8
 * Purpose: Splice builtin "set-car!": RefOpSetCadr(...,0x23)
 * Info: proposed name RefOpSetCar (T1); TU bif
 * Confidence: high [T1 0.93] - evidence: static analysis; sources: r1_116918+verdicts_B+verify_side
 * See: docs/engine/splice-scripting.md#7-key-functions,
 *   docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef *func_00120CC8(CRef *ret, void *pbasic, int cref, CRef *aref)
{
    func_00120C48(ret, pbasic, cref, aref, 0x23);
    return ret;
}
