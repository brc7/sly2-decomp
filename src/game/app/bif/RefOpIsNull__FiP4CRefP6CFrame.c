typedef struct CRef { int m_tagk; int m_tag; } CRef;
/** @sly2-annot addr=0x120990
 * Purpose: Splice builtin "null?": tag==0
 * Info: proposed name RefOpIsNull (T1); TU bif
 * Confidence: high [T1 0.93] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef *RefOpIsNull__FiP4CRefP6CFrame(CRef *ret, void *pbasic, CRef *pref)
{
    int tagk = pref->m_tagk;
    ret->m_tagk = 0x17;
    ret->m_tag = tagk == 0;
    return ret;
}
