typedef struct CRef { int m_tagk; int m_tag; } CRef;
void Decref__4CRef(CRef *pref);
/** @sly2-annot addr=0x129970
 * Purpose: Incref new node, Decref old, tag 0x1F
 * Info: proposed name CRef_SetPair (T1); class CRef; TU ref
 * Notes: names_v2 v1 proposed SetMatrix__4CRefP7MATRIX4 (0.050)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#ref-0x1291a8-0x12a3d8
 * @end-sly2-annot */
CRef *func_00129970(CRef *pref, int *p)
{
    (*p)++;
    Decref__4CRef(pref);
    pref->m_tag = (int)p;
    pref->m_tagk = 0x1F;
    return pref;
}
