struct VTE { short d; short pad; void (*pfn)(void *); };
struct O { char pad[0xC]; struct VTE *vt; char pad2[0x28]; int n38; char pad3[0xC0 - 0x3C]; int nC0; int pad4; int nC8; };
struct X { char pad[0xC]; int nC; };
struct T { char pad[0x294]; int n294; char pad2[0x3CC - 0x298]; struct X *px; };
struct S { char pad[0x10]; struct T *p10; int pad2; char a18[0x18]; int n30; };
void PopBhv(void *p);
struct O *func_00205438(void *p, int n);
void func_00178D90(struct O *o, int a, int b);
void func_0017C550(struct S *p, int n) {
    if (n == p->n30) return;
    PopBhv(p->a18);
    p->n30 = n;
    switch (n) {
    case 0:
        { struct O *o = func_00205438(p->p10, 0x9E); struct VTE *e; o->n38 = p->p10->px->nC; e = &o->vt[8]; e->pfn((char *)o + e->d); }
        break;
    case 1:
        { struct O *o = func_00205438(p->p10, 0xA7); struct VTE *e; func_00178D90(o, p->p10->n294, 0); o->nC0 = 8; o->nC8 = p->p10->px->nC; e = &o->vt[8]; e->pfn((char *)o + e->d); }
        break;
    }
}
