typedef int qword __attribute__((mode(TI)));
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
struct OBJ { int a[4]; OBJ(); ~OBJ(); };
extern char D_003DD490[];
extern "C" {
void IntersectSwBoundingSphere(void *pg, qword a, void *p3, void *p4, float g, int *pc, void **pap);
void func_0020FDC8(int c, void *ap, int *pc, void **pap);
CRef RefPairFromAplo__FiPP2LO(int c, void *ap);
CRef RefOpFindObjectsInBoundingSphere__FiP4CRefP6CFrame(void *pbasic, CRef *aref);
}
CRef RefOpFindObjectsInBoundingSphere__FiP4CRefP6CFrame(void *pbasic, CRef *aref)
{
    CRef ref;
    float g;
    if (aref[1].m_tagk == 0x15)
        g = (float)aref[1].u.n;
    else
        g = aref[1].u.g;
    OBJ obj;
    int c0;
    void *ap0;
    int c1;
    void *ap1;
    IntersectSwBoundingSphere(D_003DD490, *(qword *)aref[0].u.pv, 0, 0, g, &c0, &ap0);
    func_0020FDC8(c0, ap0, &c1, &ap1);
    ref = RefPairFromAplo__FiPP2LO(c1, ap1);
    return ref;
}
