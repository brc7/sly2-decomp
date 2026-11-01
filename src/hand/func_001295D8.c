typedef struct CRef { int m_tagk; int m_tag; } CRef;
void Decref__4CRef(CRef *pref);
/** @sly2-annot addr=0x1295D8
 * Purpose: Decref then tag 0x18
 * Info: proposed name CRef_SetSymid (T1); class CRef; TU ref
 * Notes: names_v2 v1 proposed SetProc__4CRefP5CProc (0.000)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r1_116918+verify_side
 * See: docs/reference/functions/splice-scripting.md#ref-0x1291a8-0x12a3d8
 * @end-sly2-annot */
CRef *func_001295D8(CRef *pref, int n)
{
    Decref__4CRef(pref);
    pref->m_tag = n;
    pref->m_tagk = 0x18;
    return pref;
}
