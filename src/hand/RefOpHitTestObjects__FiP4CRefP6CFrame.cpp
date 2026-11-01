struct CRef {
    int m_tagk; union { int n; float g; void *pv; } u;
    CRef() { m_tagk = -1; }
    CRef(const CRef &r);
    void Decref();
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &r);
};
extern "C" {
CRef RefOpHitTestObjectsImpl__F4BIFKiP4CRefP6CFrame(int k, void *a, void *b, void *c);
CRef RefOpHitTestObjects__FiP4CRefP6CFrame(void *a, void *b, void *c);
}
CRef RefOpHitTestObjects__FiP4CRefP6CFrame(void *a, void *b, void *c)
{
    CRef ref;
    ref = RefOpHitTestObjectsImpl__F4BIFKiP4CRefP6CFrame(0x7B, a, b, c);
    return ref;
}
