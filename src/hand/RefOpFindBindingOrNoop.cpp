struct CRef {
    int m_tagk; union { int n; float g; void *pv; } u;
    CRef() { m_tagk = -1; }
    CRef(int tagk) { InitTag(tagk); }
    CRef(int n, int tagk) { m_tagk = tagk; u.n = n; }
    CRef(const CRef &r);
    void Decref();
    void InitTag(int tagk);
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &r);
    CRef &operator=(float g);
};
struct O { int pad[2]; int k; O *pparent; int pad2[5]; int f; };
class CFrame { public: CRef *PrefFindBinding(unsigned int sym, int fRecurse); };
extern "C" CRef RefOpFindBindingOrNoop(void *pbasic, CRef *aref, O *po);
CRef RefOpFindBindingOrNoop(void *pbasic, CRef *aref, O *po)
{
    if (po && po->k == 1) {
        while (!po->f) {
            po = po->pparent;
            if (!po || po->k != 1)
                goto fail;
        }
        {
            CRef *pref = ((CFrame *)po->pparent)->PrefFindBinding(aref[0].u.n, 1);
            if (pref)
                return *pref;
        }
    }
fail:
    return CRef(0x86, 0x19);
}
