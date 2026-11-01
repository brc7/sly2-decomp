struct VTE { short d; short p; void (*pfn)(void *); };
struct VT { struct VTE a[18]; };
struct X { char pad[0xC]; struct VT *pvt; };
struct E { int fl; struct X *px; };
struct P { char pad[0x3F14]; int c; int pad2; int *pi; struct E *ae; };
void RecalcSwXpAll(struct P *);
void func_001F8F80(struct P *, int);
void func_001F8DD8(struct P *p)
{
    int i;
    RecalcSwXpAll(p);
    p->pi = &i;
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->ae[i];
        if ((pe->fl & 0x24) == 4) pe->px->pvt->a[17].pfn((char *)pe->px + pe->px->pvt->a[17].d);
    }
    p->pi = 0;
    func_001F8F80(p, 1);
    RecalcSwXpAll(p);
    func_001F8F80(p, 6);
}
