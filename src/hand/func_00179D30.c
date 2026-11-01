struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
struct VTE { short d; short pad; void (*pfn)(void *); };
struct O { char pad[0xC]; struct VTE *vt; char pad2[0x2C]; int n3C; };
struct S { char pad[0x10]; char *p10; int pad2; void *p18; char pad3[0x14]; float g30; float g34; int n38; float g3C; void *p40; };
void PopBhv(void *p);
struct O *func_00205438(void *p, int n);
void func_00171C18(void *p, void *q);
void func_00171258(struct O *o, void *t, int a, void *u);
void func_00179EF8(struct S *p);
void func_00179E80(struct S *p);
void func_00179D30(struct S *p, int n) {
    if (n == p->n38) return;
    if (p->n38 == 0) func_00171C18(p->p18, p->p10 + 0x2DC);
    PopBhv(&p->p18);
    p->n38 = n;
    p->g3C = D_002DDED0.t;
    switch (n) {
    case 1: case 2:
        { struct O *o = func_00205438(p->p10, 0x9E); struct VTE *e = &o->vt[8]; e->pfn((char *)o + e->d); }
        func_00179EF8(p);
        p->g30 = 3.40282346638528859812e+38f;
        break;
    case 0:
        { struct O *o = func_00205438(p->p10, 0xA9); struct VTE *e; func_00171258(o, p->p40, 0, p->p10 + 0x2DC); e = &o->vt[8]; e->pfn((char *)o + e->d); }
        func_00179E80(p);
        p->g34 = 3.40282346638528859812e+38f;
        break;
    case 3:
        { struct O *o = func_00205438(p->p10, 0x9E); struct VTE *e; o->n3C = 1; e = &o->vt[8]; e->pfn((char *)o + e->d); }
        break;
    }
}
