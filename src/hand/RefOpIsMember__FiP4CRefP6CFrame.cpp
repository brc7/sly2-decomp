struct CRef {
    int m_tagk; union { int n; float g; void *pv; } u;
    CRef() { m_tagk = -1; }
    CRef(bool f) { m_tagk = 0x17; u.n = f; }
    CRef(const CRef &r);
    void Decref();
    void InitTag(int tagk);
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &r);
};
struct NODE { int x0, x4; CRef ref; NODE *pnext; };
extern "C" {
CRef RefOpEqual__FiP4CRefP6CFrame(int carg, CRef *aref, void *pctx);
CRef RefOpIsMember__FiP4CRefP6CFrame(int carg, CRef *aref, void *pctx);
}
CRef RefOpIsMember__FiP4CRefP6CFrame(int carg, CRef *aref, void *pctx)
{
    CRef ref;
    CRef arefT[2];
    CRef res(false);
    CRef *pres = &res;
    if (aref[1].m_tagk == 0x1F) {
        NODE *p;
        arefT[0] = aref[0];
        for (p = (NODE *)aref[1].u.pv; p; p = p->pnext) {
            arefT[1] = p->ref;
            ref = RefOpEqual__FiP4CRefP6CFrame(2, arefT, pctx);
            if (ref.m_tagk == 0x17 && ref.u.n) {
                pres->u.n = 1;
                break;
            }
        }
    }
    return res;
}
