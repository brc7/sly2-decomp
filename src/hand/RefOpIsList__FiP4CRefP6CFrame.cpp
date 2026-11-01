struct CRef {
    int m_tagk;
    int m_tag;
    CRef() {}
    CRef(bool f) { m_tagk = 0x17; m_tag = f; }
    CRef(const CRef &);
};
extern "C" CRef RefOpIsList__FiP4CRefP6CFrame(void *pbasic, CRef *pref);

/** @sly2-annot addr=0x120960
 * Purpose: Splice builtin "list?": tag==0x1F||tag==0
 * Info: proposed name RefOpIsList (T1); TU bif
 * Confidence: high [T1 0.93] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef RefOpIsList__FiP4CRefP6CFrame(void *pbasic, CRef *pref)
{
    int tagk = pref->m_tagk;
    return CRef(tagk == 0x1F || tagk == 0);
}
