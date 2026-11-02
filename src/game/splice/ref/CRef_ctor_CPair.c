typedef struct CRef { int m_tagk; int m_tag; } CRef;
/** @sly2-annot addr=0x129480
 * Purpose: Constructor: tag 0x1F, stores node pointer and increments its refcount
 * Info: proposed name CRef_ctor_CPair (T1); class CRef; TU ref
 * Notes: C++ signature hint (CPair *) | COLLISION: CRef_ctor already used (same name proposed for 0x129248);
 *   renamed to CRef_ctor_CPair
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#ref-0x1291a8-0x12a3d8
 * @end-sly2-annot */
CRef *CRef_ctor_CPair(CRef *ret, int *p)
{
    ret->m_tag = (int)p;
    ret->m_tagk = 0x1F;
    (*p)++;
    return ret;
}
