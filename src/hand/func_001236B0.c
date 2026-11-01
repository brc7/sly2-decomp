typedef struct CRef { int m_tagk; float m_g; } CRef;
/** @sly2-annot addr=0x1236B0
 * Purpose: Splice builtin "round": (int)(g+0.5)
 * Info: proposed name RefOpRound (T1); TU bif
 * Confidence: high [T1 0.93] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef *func_001236B0(CRef *ret, void *pbasic, CRef *aref)
{
    float g = aref->m_g;
    ret->m_tagk = 0x15;
    *(int *)&ret->m_g = (int)(g + 0.5f);
    return ret;
}
