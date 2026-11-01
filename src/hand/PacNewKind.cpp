struct AREQ { int cb; int cbAlign; };
extern "C" void *func_001AE2A0(AREQ *preq);
struct CBase {
    void *operator new(unsigned int cb) { AREQ req; req.cb = cb; req.cbAlign = 0x10; return func_001AE2A0(&req); }
    virtual void F();
};
struct C0 : CBase { int a[3]; virtual void F(); };
struct C1 : CBase { int a[2]; virtual void F(); };
struct C2 : CBase { int a[2]; virtual void F(); };
struct C3 : CBase { int a[11]; virtual void F(); };
struct C4 : CBase { int a[11]; virtual void F(); };
struct C5 : CBase { int a[7]; virtual void F(); };
extern "C" CBase *PacNewKind(int k);
CBase *PacNewKind(int k)
{
    switch (k) {
    case 0: return new C0;
    case 1: return new C1;
    case 2: return new C2;
    case 3: return new C3;
    case 4: return new C4;
    case 5: return new C5;
    }
    return 0;
}
