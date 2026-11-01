struct AREQ { int cb; int cbAlign; };
extern "C" void *func_001AE2A0(AREQ *preq);
struct CBase {
    void *operator new(unsigned int cb) { AREQ req; req.cb = cb; req.cbAlign = 0x10; return func_001AE2A0(&req); }
    virtual void F();
};
struct C0 : CBase { int a; virtual void F(); };
struct C1 : CBase { int a, b; virtual void F(); };
struct C2 : CBase { int a, b, c, d; virtual void F(); };
extern "C" CBase *PacNewKind2(int k);
CBase *PacNewKind2(int k)
{
    switch (k) {
    case 0: return new C0;
    case 1: return new C1;
    case 2: return new C2;
    }
    return 0;
}
