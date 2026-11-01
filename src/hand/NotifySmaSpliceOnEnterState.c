struct VTE { short d; short p; void *pfn; };
struct VT { struct VTE a[5]; };
struct P { char pad[0xC]; struct VT *pvt; char pad2[0x24 - 0x10]; void *p24; };
struct L { struct P *p; int a; int b; };
int OidFromSmIsms(void *, int);
void NotifySmaSpliceOnEnterState(struct P *p, int a, int b)
{
    struct L l;
    int *ap[2];
    l.p = p;
    l.a = OidFromSmIsms(p->p24, a);
    l.b = OidFromSmIsms(p->p24, b);
    ((void (*)(void *, int, struct L *))p->pvt->a[4].pfn)((char *)p + p->pvt->a[4].d, 10, &l);
    ap[0] = &l.a;
    ap[1] = &l.b;
    ((void (*)(void *, int, int, int, int **, int))p->pvt->a[3].pfn)((char *)p + p->pvt->a[3].d, 8, 0, 2, ap, 0);
}
