struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
struct VTE { short d; short pad; void (*pfn)(void *); };
struct O { char pad[0xC]; struct VTE *vt; char pad2[0x2C]; int n3C; };
struct T { char pad[0x32C]; int n32C; };
struct S { char pad[0x10]; struct T *p10; int pad2; char a18[0x18]; int n30; float g34; };
void PopBhv(void *p);
void ClearBhvBusy(struct S *p);
struct O *func_00205438(void *p, int n);
void func_0017A8C8(struct S *p, int n) {
    if (p->n30 == n) return;
    PopBhv(p->a18);
    p->n30 = n;
    p->g34 = D_002DDED0.t;
    switch (n) {
    case 0: case 1: case 2: case 3: case 4:
        {
            struct O *o = func_00205438(p->p10, 0x9D);
            int a[5] = { 18, 19, 20, 21, 19 };
            struct VTE *e;
            o->n3C = a[p->n30];
            e = &o->vt[8]; e->pfn((char *)o + e->d);
        }
        p->p10->n32C = (unsigned)p->n30 < 2;
        break;
    case -1:
        ClearBhvBusy(p);
        p->p10->n32C = 0;
        break;
    }
}
