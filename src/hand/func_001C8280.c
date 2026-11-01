struct S { char pad[0x40]; int x40; char pad2[0x13C]; int n180; char pad3[0x10]; int n194; char pad4[0x58]; int n1f0, n1f4, n1f8, n1fc, n200; };
void InitPo(struct S *p);
static inline int FPtr(int v) { return !(v & 1); }
void func_001C8280(struct S *p)
{
    int v;
    InitPo(p);
    v = p->x40;
    p->n1f0 = 0x7B;
    p->n1fc = -1;
    p->n1f8 = -1;
    p->n1f4 = -1;
    p->n200 = -1;
    p->n180 = -1;
    p->n194 = -1;
    if (FPtr(v)) *(int *)v |= 2; else p->x40 = v | 2;
}
