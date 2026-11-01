struct CRef {
    int m_tagk;
    int m_tag;
    CRef() {}
    CRef(bool f) { m_tagk = 0x17; m_tag = f; }
    CRef(const CRef &);
};
extern "C" CRef func_00120A20(void *pbasic, CRef *pref);

/** @sly2-annot addr=0x120A20
 * Purpose: Splice builtin "procedure?": tag==0x20||0x19
 * Info: proposed name RefOpIsProcedure (T1); TU bif
 * Confidence: high [T1 0.93] - evidence: static analysis; sources: r1_116918+verdicts_tu_r1r2
 * See: docs/engine/splice-scripting.md#7-key-functions,
 *   docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef func_00120A20(void *pbasic, CRef *pref)
{
    int tagk = pref->m_tagk;
    return CRef(tagk == 0x20 || tagk == 0x19);
}
