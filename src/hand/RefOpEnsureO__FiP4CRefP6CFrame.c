typedef struct CRef { int m_tagk; int m_tag; } CRef;
void RefOpObjectOption__FiP4CRefP6CFrame4BIFK(CRef *ret, void *pbasic, int cref, CRef *aref, int k);
/** @sly2-annot addr=0x123FE8
 * Purpose: Splice builtin "ensureo": op 0x6D RefEnsureOption
 * Info: proposed name RefOpEnsureO (T1); TU bif
 * Confidence: high [T1 0.93] - evidence: static analysis; sources: r1_116918+verdicts_A1
 * See: docs/engine/splice-scripting.md#7-key-functions,
 *   docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef *RefOpEnsureO__FiP4CRefP6CFrame(CRef *ret, void *pbasic, int cref, CRef *aref)
{
    RefOpObjectOption__FiP4CRefP6CFrame4BIFK(ret, pbasic, cref, aref, 0x6D);
    return ret;
}
