struct VTE { short d; short pad; void (*pfn)(void *); };
struct O { char pad[0xC]; struct VTE *vt; };
struct S { char pad[0x54]; int n; float t; char pad2[0x78 - 0x5C]; float dt; char pad3[4]; struct O *po; };
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
extern int D_003E0F9C;
void UpdateLo(struct S *p);
void func_00165118(struct S *p, int n);
void UpdateTask(struct S *p)
{
    struct O *po;
    UpdateLo(p);
    if (p->n == 1 && p->dt > 0.0f) {
        float dt = D_002DDED0.t - p->t;
        if (dt < p->dt && p->dt + D_002DDED0.dt <= dt)
            func_00165118(p, 0);
    }
    po = p->po;
    if (po) {
        int f = p->n == 1; if (D_003E0F9C) f = 0;
        if (f) {
            struct VTE *e = &po->vt[8];
            e->pfn((char *)po + e->d);
        } else {
            struct VTE *e = &po->vt[9];
            e->pfn((char *)po + e->d);
        }
    }
}
