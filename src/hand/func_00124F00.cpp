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
CRef func_00124F00(void *a, void *b, void *c);
}
/** @sly2-annot addr=0x124F00
 * Purpose: Splice builtin "hit-test-objects": RefOpHitTestObjectsImpl(0x7B=own BIFK) -> list of (lo pos
 *   normal)
 * Info: proposed name RefOpHitTestObjects (T1); TU bif
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r1_116918+verdicts_tu_r1r2
 * See: docs/engine/splice-scripting.md#7-key-functions,
 *   docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef func_00124F00(void *a, void *b, void *c)
{
    CRef ref;
    ref = func_001249A8(0x7B, a, b, c);
    return ref;
}
