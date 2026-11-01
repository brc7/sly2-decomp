struct M { int cb; int a; };
struct S { char pad[0x144]; int x; int c; int *a; };
void LoadVolFromBrx(struct S *p, void *q);
void func_00228250(void *q, int *p);
int func_00117BD0(void *q);
int *func_001AE1D0(struct M *pm);
void func_002282B0(struct S *p, void *q)
{
    int i = 0;
    int n;
    struct M m;
    LoadVolFromBrx(p, q);
    func_00228250(q, &p->x);
    n = func_00117BD0(q);
    m.a = 0x10;
    m.cb = n * 4;
    p->c = n;
    p->a = func_001AE1D0(&m);
    for (; i < p->c; i++) func_00228250(q, &p->a[i]);
}
