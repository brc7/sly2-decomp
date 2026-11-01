struct KV { int k; int v; };
struct T { int c; struct KV *a; };
extern struct T D_00279AD0[];
struct R { int n0; int n4; };
struct VTE { short d; short p; struct R *(*pfn)(void *); };
struct VT { struct VTE a[7]; };
struct X { char pad[0xC]; struct VT *pvt; };
int PeopidFind(struct X *px, int k)
{
    struct R *r = px->pvt->a[6].pfn((char *)px + px->pvt->a[6].d);
    struct T *t = &D_00279AD0[r->n4];
    int lo = 0, hi = t->c - 1;
    struct KV *a = t->a;
    while (lo <= hi) {
        int mid = (lo + hi) >> 1;
        if (a[mid].k == k) return a[mid].v;
        if (k < a[mid].k) hi = mid - 1;
        else lo = mid + 1;
    }
    return 0;
}
