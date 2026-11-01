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
    CRef &operator=(int n);
};
extern "C" {
CRef func_00129DA8(CRef *pref);
int func_0011D3F8(int a, int b);
float func_0011D448(float a, float b);
CRef func_001223C0(void *pbasic, CRef *aref);
}
/** @sly2-annot addr=0x1223C0
 * Purpose: Splice builtin "random": NRandInRange for ints else GRandInRange
 * Info: proposed name RefOpRandom (T1); TU bif
 * Confidence: high [T1 0.93] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef func_001223C0(void *pbasic, CRef *aref)
{
    CRef ref;
    if (aref[0].m_tagk == 0x15 && aref[1].m_tagk == aref[0].m_tagk) {
        ref = func_0011D3F8(aref[0].u.n, aref[1].u.n);
    } else {
        float g0 = func_00129DA8(&aref[0]).u.g;
        float g1 = func_00129DA8(&aref[1]).u.g;
        ref = func_0011D448(g0, g1);
    }
    return ref;
}
