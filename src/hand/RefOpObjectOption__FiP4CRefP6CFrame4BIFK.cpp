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
CRef func_00200CA8(void *a, void *b, CRef *pref);
CRef func_00200D10(void *a, void *b, CRef *pref);
CRef func_00200C28(void *a, void *b, CRef *pref);
CRef RefGetOption__FP5BASICi(void *a, void *b);
CRef RefOpObjectOption__FiP4CRefP6CFrame4BIFK(void *pbasic, CRef *aref, void *pv, int op);
}
CRef RefOpObjectOption__FiP4CRefP6CFrame4BIFK(void *pbasic, CRef *aref, void *pv, int op)
{
    CRef ref;
    void *a = aref[0].u.pv;
    void *b = aref[1].u.pv;
    switch (op) {
    case 0x6C:
        ref = func_00200CA8(a, b, &aref[2]);
        break;
    case 0x6D:
        ref = func_00200D10(a, b, &aref[2]);
        break;
    case 0x6E:
        ref = func_00200C28(a, b, &aref[2]);
        break;
    case 0x6F:
        ref = RefGetOption__FP5BASICi(a, b);
        break;
    }
    return ref;
}
