typedef struct CRef { int m_tagk; float m_g; } CRef;
extern float D_002DDED0;
/** @sly2-annot addr=0x123A08
 * Purpose: Splice builtin "current-time": F32 = g_clock.t (D_002DDED0)
 * Info: proposed name RefOpCurrentTime (T1); TU bif
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef *RefOpCurrentTime__FiP4CRefP6CFrame(CRef *ret)
{
    ret->m_tagk = 0x16;
    ret->m_g = D_002DDED0;
    return ret;
}
