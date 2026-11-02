typedef struct CRef { int m_tagk; int m_tag; } CRef;
/** @sly2-annot addr=0x1294A0
 * Purpose: Constructor: tag 0x20 (proc), increments refcount
 * Info: proposed name CRef_ctor_CProc (T1); class CRef; TU ref
 * Notes: C++ signature hint (CProc *) | COLLISION: CRef_ctor already used (same name proposed for 0x129248);
 *   renamed to CRef_ctor_CProc
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#ref-0x1291a8-0x12a3d8
 * @end-sly2-annot */
CRef *CRef_ctor_CProc(CRef *ret, int *p)
{
    ret->m_tag = (int)p;
    ret->m_tagk = 0x20;
    (*p)++;
    return ret;
}
