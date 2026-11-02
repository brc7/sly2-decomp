typedef struct CRef { int m_tagk; union { int n; float g; } u; } CRef;
CRef *__4CRefRC4CRef(CRef *pref, CRef *prefSrc);
/** @sly2-annot addr=0x129D58
 * Purpose: Returns S32 copy (float truncated)
 * Info: proposed name CRef_RefCoerceS32 (T1); class CRef; TU ref
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#ref-0x1291a8-0x12a3d8
 * @end-sly2-annot */
CRef *CRef_RefCoerceS32(CRef *pref, CRef *prefSrc)
{
    if (prefSrc->m_tagk == 0x15) {
        __4CRefRC4CRef(pref, prefSrc);
    } else {
        float g = prefSrc->u.g;
        pref->m_tagk = 0x15;
        pref->u.n = (int)g;
    }
    return pref;
}
