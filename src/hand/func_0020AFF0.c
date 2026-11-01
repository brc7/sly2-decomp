struct X { int a; };
struct E { int n; struct X *px; int b; int c; };
struct S { char pad[0xAC]; int nAC; struct E e; };
void ReleaseAnimBinding(struct E *pe, int n);
struct X *func_0020C010(int cb);
void func_0020BD78(struct X *px);
void func_0020AFF0(struct S *p, int a)
{
    struct E *pe = &p->e;
    ReleaseAnimBinding(pe, p->nAC);
    p->e.n = 2;
    pe->px = func_0020C010(0x40);
    func_0020BD78(pe->px);
    pe->px->a = a;
    pe->c = 0;
}
