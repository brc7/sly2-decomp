typedef struct CRef { int m_tagk; int m_tag; } CRef;
void Decref__4CRef(CRef *pref);
CRef *__4CRefRC4CRef(CRef *pref, CRef *prefSrc);
/** @sly2-annot addr=0x125B70
 * Purpose: Stubbed Splice builtin: returns Nil (tagk -1); one of 29 identical 60-byte stubs
 * Info: proposed name RefOpStub150 (T2); TU bif2
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#bif2-0x1258f0-0x126188
 * @end-sly2-annot */
CRef *func_00125B70(CRef *ret)
{
    CRef ref;
    ref.m_tagk = -1;
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
