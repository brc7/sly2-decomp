class O {
public:
    int m_a[3];
    virtual void AddRef();
    virtual void Release();
    virtual void V3(int a, int b, int c, int d, int e);
    virtual void V4(int a, O *p);
};
struct DL;
extern DL D_003E148C;
extern "C" int func_00156870(O *o);
extern "C" void RemoveDlEntry__FP2DLPv(DL *pdl, void *pv);
class CRefGuard {
public:
    O *m_p;
    CRefGuard(O *p) : m_p(p) { p->AddRef(); }
    ~CRefGuard() { m_p->Release(); }
};
extern "C" void func_00156780(O *o)
{
    if (func_00156870(o))
        return;
    CRefGuard guard(o);
    o->V4(0xF, o);
    if (func_00156870(o))
        return;
    RemoveDlEntry__FP2DLPv(&D_003E148C, o);
    o->V3(3, 0, 0, 0, 0);
    o->Release();
}
