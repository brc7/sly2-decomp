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
CRef CRef_RefCoerceF32(CRef *pref);
int NRandInRange__Fii(int a, int b);
float GRandInRange__Fff(float a, float b);
CRef RefOpRandom__FiP4CRefP6CFrame(void *pbasic, CRef *aref);
}
/** @sly2-annot addr=0x1223C0
 * Purpose: Splice builtin "random": NRandInRange for ints else GRandInRange
 * Info: proposed name RefOpRandom (T1); TU bif
 * Confidence: high [T1 0.93] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef RefOpRandom__FiP4CRefP6CFrame(void *pbasic, CRef *aref)
{
    CRef ref;
    if (aref[0].m_tagk == 0x15 && aref[1].m_tagk == aref[0].m_tagk) {
        ref = NRandInRange__Fii(aref[0].u.n, aref[1].u.n);
    } else {
        float g0 = CRef_RefCoerceF32(&aref[0]).u.g;
        float g1 = CRef_RefCoerceF32(&aref[1]).u.g;
        ref = GRandInRange__Fff(g0, g1);
    }
    return ref;
}
