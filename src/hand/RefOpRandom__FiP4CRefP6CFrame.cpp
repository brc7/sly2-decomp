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
