struct CRef {
    int m_tagk; union { int n; float g; void *pv; } u;
    CRef() { m_tagk = -1; }
    CRef(int tagk) { InitTag(tagk); }
    CRef(const CRef &r);
    void Decref();
    void InitTag(int tagk);
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &r);
    CRef &operator=(float g);
};
extern "C" {
CRef CRef_RefCoerceF32(CRef *pref);
void FitClq__FffffP3CLQ(void *pv, float a, float b, float c, float d);
CRef RefOpClqFit__FiP4CRefP6CFrame(void *pbasic, CRef *aref);
}
/** @sly2-annot addr=0x123498
 * Purpose: Splice builtin "clq-fit": FitClq
 * Info: proposed name RefOpClqFit (T1); TU bif
 * Confidence: high [T1 0.93] - evidence: static analysis; sources: r1_116918+verdicts_A1
 * See: docs/engine/splice-scripting.md#7-key-functions,
 *   docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef RefOpClqFit__FiP4CRefP6CFrame(void *pbasic, CRef *aref)
{
    CRef ref(0x1C);
    {
        CRef r0 = CRef_RefCoerceF32(&aref[0]);
        float g0 = r0.u.g;
        CRef r1 = CRef_RefCoerceF32(&aref[1]);
        float g1 = r1.u.g;
        CRef r2 = CRef_RefCoerceF32(&aref[2]);
        float g2 = r2.u.g;
        CRef r3 = CRef_RefCoerceF32(&aref[3]);
        FitClq__FffffP3CLQ(ref.u.pv, g0, g1, g2, r3.u.g);
    }
    return ref;
}
