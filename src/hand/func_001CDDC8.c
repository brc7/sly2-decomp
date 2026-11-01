struct S { char p0[0x40]; int x40; char p1[0x148 - 0x44]; unsigned long x148; int x150; char p2[0x15C - 0x154]; char dl[4]; };
void func_001F2108(struct S *p);
void InitDl__FP2DLi(void *pdl, int n);
void SetLoBusyk(struct S *p, int n);
void func_001F27F0(struct S *p);
void func_001F6860(struct S *p, int n);
static inline int FPtr(int v) { return !(v & 1); }
void func_001CDDC8(struct S *p)
{
    int v;
    func_001F2108(p);
    InitDl__FP2DLi(p->dl, 0xC);
    SetLoBusyk(p, 1);
    v = p->x40;
    if (FPtr(v)) *(int *)v = (*(int *)v & 1) | 0xE; else p->x40 = (v & 1) | 0xE;
    p->x148 &= ~(1UL << 35);
    if (p->x150 == 1) {
        func_001F27F0(p);
        func_001F6860(p, 1);
    }
}
