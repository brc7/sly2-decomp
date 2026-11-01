typedef int qword __attribute__((mode(TI)));
struct VTE { short d; short pad; void (*pfn)(); };
struct O { int a[3]; struct VTE *vt; };
struct P { int a; int b; float f; float g; qword q; qword r; qword s; };
void ExplodeExplParams(struct O *o, int a, int b, float f, float g, qword q, qword r)
{
    struct P p;
    struct VTE *e;
    p.a = a;
    p.b = b;
    p.f = f;
    p.g = g;
    p.q = q;
    p.r = r;
    e = &o->vt[51];
    e->pfn((char *)o + e->d, &p);
}
