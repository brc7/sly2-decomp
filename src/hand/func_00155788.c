typedef struct CLOCK { float t; float dt; } CLOCK;
extern CLOCK D_002DDED0;
struct VTE { short d; short pad; void (*pfn)(); };
struct O { int a[3]; struct VTE *vt; char pad[0x20]; int n30; int pad34; void *p38; int pad3c; int n40; char pad3[8]; int n4C; };
struct T { char pad[0x33C]; int n33C; char pad2[0x1018 - 0x340]; int n1018; };
struct Q { char pad[0x20]; int n20; };
struct S { int pad[4]; struct T *pt; int pad2; char ab18[0x18]; int k30; float t34; int n38; struct Q *pq; };
void PopBhv(void *p);
struct O *func_00205438(struct T *pt, int n);
void func_00155788(struct S *p, int k)
{
    if (k == p->k30)
        return;
    if (p->k30 == 0)
        p->pq->n20 = 1;
    PopBhv(p->ab18);
    p->k30 = k;
    p->t34 = D_002DDED0.t;
    switch (k) {
    case 0: {
        struct O *o = func_00205438(p->pt, 0xB5);
        struct VTE *e;
        o->p38 = p->pq;
        o->n4C = p->n38;
        e = &o->vt[8];
        e->pfn((char *)o + e->d);
        p->pt->n1018 = p->pt->n33C;
        break;
    }
    case 1: {
        struct O *o = func_00205438(p->pt, 0xA5);
        struct VTE *e;
        o->n30 = p->n38;
        o->n40 = k;
        e = &o->vt[8];
        e->pfn((char *)o + e->d);
        break;
    }
    }
}
