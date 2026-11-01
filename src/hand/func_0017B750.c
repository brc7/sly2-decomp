struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
struct VTE { short d; short pad; void (*pfn)(void *); };
struct O { char pad[0xC]; struct VTE *vt; char pad2[0x28]; int n38; int n3C; };
struct T { char pad[0xF60]; int nF60; };
struct S { char pad[0x10]; struct T *pt; int pad2; char a18[0x18]; int n30; float g34; int n38; };
void PopBhv(void *p);
struct O *func_00205438(void *p, int n);
void func_0017B750(struct S *p, int n) {
    if (n == p->n30) return;
    PopBhv(p->a18);
    if (!p->n30) p->pt->nF60 = 0;
    p->n30 = n;
    p->g34 = D_002DDED0.t;
    if (!n) {
        struct O *o = func_00205438(p->pt, 0x9D);
        struct VTE *e;
        o->n3C = 0x11;
        o->n38 = p->n38;
        e = &o->vt[8];
        e->pfn((char *)o + e->d);
        p->pt->nF60 = 1;
    }
}
