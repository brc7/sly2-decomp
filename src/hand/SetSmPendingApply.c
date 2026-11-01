struct T { int a, b, c; };
struct P { char pad[0x74]; struct T *pt; };
struct CB { int cb; int n; };
struct T *func_001AE1D0(struct CB *);
void SetSmPendingApply(struct P *p, int a, int b, int c)
{
    struct CB cb;
    struct T *pt;
    cb.cb = 0xC; cb.n = 0x10;
    pt = func_001AE1D0(&cb);
    p->pt = pt;
    pt->a = a; pt->b = b; pt->c = c;
}
