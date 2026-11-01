typedef struct { short d; short pad; void *pfn; } VTE;
struct O { char pad[0xC]; VTE *vt; };
struct S { char pad[0x60]; struct O *po; char pad2[0xC]; long fl; };
struct B { char pad[0xEC]; struct S *ps; };
struct E { int pad; struct B *pb; int pad2[2]; long fl; };
int func_00134310(int a, int b);
#define VC61(o, a) ((int (*)(void *, struct O *))(o)->vt[61].pfn)((char *)(o) + (o)->vt[61].d, a)
int FFilterSoPair(struct S *p, struct E *e)
{
    struct S *ps = e->pb->ps;
    int a, b, r;
    if (!(e->fl & (1L << 48)) && (ps->fl & 4))
        return 0;
    if ((e->fl & (1L << 48)) || ps == p)
        return 0;
    a = VC61(p->po, ps->po);
    b = VC61(ps->po, p->po);
    r = func_00134310(a, b);
    return r != 1 && r != 3;
}
