typedef struct CRef { int m_tagk; int m_tag; } CRef;
void Decref__4CRef(CRef *pref);
/** @sly2-annot addr=0x129518
 * Purpose: Decref then tag 0x15 = n
 * Info: proposed name CRef_SetS32 (T1); class CRef; TU ref
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources:
 *   r1_116918+verdicts_A1+verify_side+names_v2-signature(verified)
 * See: docs/engine/splice-scripting.md#7-key-functions,
 *   docs/reference/functions/splice-scripting.md#ref-0x1291a8-0x12a3d8
 * @end-sly2-annot */
CRef *SetS32__4CRefi(CRef *pref, int n)
{
    Decref__4CRef(pref);
    pref->m_tag = n;
    pref->m_tagk = 0x15;
    return pref;
}
