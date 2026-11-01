typedef struct CRef { int m_tagk; int m_tag; } CRef;
/** @sly2-annot addr=0x120370
 * Purpose: Splice builtin "=": bool(tag0==tag1 payload ints equal)
 * Info: proposed name RefOpIntEqual (T1); TU bif
 * Notes: verifier: purpose partly wrong: BIF entry 7 = Sly 1 '=' (RefOpIntEqual). Purpose implies tag0==tag1,
 *   but the code compares only the two payload words (xor+sltiu) and never checks tags.
 * Confidence: high [T1 0.93] - evidence: Sly 1 / Lilly reference; sources: r1_116918+verdicts_tu_r1r2
 * See: docs/engine/splice-scripting.md#7-key-functions,
 *   docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef *RefOpIntEqual__FiP4CRefP6CFrame(CRef *ret, void *pbasic, CRef *aref)
{
    int a = aref[0].m_tag;
    int b = aref[1].m_tag;
    ret->m_tagk = 0x17;
    ret->m_tag = a == b;
    return ret;
}
