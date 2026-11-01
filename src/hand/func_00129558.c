typedef struct CRef { int m_tagk; float m_g; } CRef;
void Decref__4CRef(CRef *pref);
/** @sly2-annot addr=0x129558
 * Purpose: Decref then tag 0x16 = g
 * Info: proposed name CRef_SetF32 (T1); class CRef; TU ref
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources:
 *   r1_116918+verdicts_A1+verify_side+names_v2-signature(verified)
 * See: docs/engine/splice-scripting.md#7-key-functions,
 *   docs/reference/functions/splice-scripting.md#ref-0x1291a8-0x12a3d8
 * @end-sly2-annot */
CRef *func_00129558(CRef *pref, float g)
{
    Decref__4CRef(pref);
    pref->m_g = g;
    pref->m_tagk = 0x16;
    return pref;
}
