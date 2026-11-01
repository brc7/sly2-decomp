struct CRef {
    int m_tagk; union { int n; float g; void *pv; } u;
    CRef() { m_tagk = -1; }
    CRef(const CRef &r);
    void Decref();
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &r);
};
extern "C" {
CRef func_001249A8(int k, void *a, void *b, void *c);
CRef func_00124F80(void *a, void *b, void *c);
}
/** @sly2-annot addr=0x124F80
 * Purpose: Splice builtin "hit-test-objects-first": Impl(0x7C) -> first hit
 * Info: proposed name RefOpHitTestObjectsFirst (T1); TU bif
 * Notes: names_v2 C proposed RefOpHitTestObjects__FiP4CRefP6CFrame (0.638)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef func_00124F80(void *a, void *b, void *c)
{
    CRef ref;
    ref = func_001249A8(0x7C, a, b, c);
    return ref;
}
