typedef struct CRef { int m_tagk; float m_g; } CRef;
/** @sly2-annot addr=0x123688
 * Purpose: Splice builtin "ceiling": (int)(g+1)
 * Info: proposed name RefOpCeiling (T1); TU bif
 * Confidence: high [T1 0.93] - evidence: static analysis; sources: r1_116918+verdicts_tu_r1r2
 * See: docs/engine/splice-scripting.md#7-key-functions,
 *   docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef *RefOpCeiling__FiP4CRefP6CFrame(CRef *ret, void *pbasic, CRef *aref)
{
    float g = aref->m_g;
    ret->m_tagk = 0x15;
    *(int *)&ret->m_g = (int)(g + 1.0f);
    return ret;
}
