struct VTE { short d; short pad; void (*pf)(); };
struct LO { char pad[0xC]; struct VTE *vt; char pad2[0x150 - 0x10]; int x150; char pad3[0x230 - 0x154]; void *x230; };
void *func_001FBB90(int n);
void func_001FB2F8(void *p, int n);
void RequestTaskState(void *p, int n, struct LO *plo);
void func_001C60B8(struct LO *p, int n);
void func_001C6B28(struct LO *p, int n)
{
    if (p->x150 == n) return;
    if (n == 1) {
        func_001FB2F8(func_001FBB90(0), 0);
        if (p->x230) RequestTaskState(p->x230, 2, p);
        n = 2;
        ((void (*)(void *, int, int, int, int, int))p->vt[3].pf)((char *)p + p->vt[3].d, 5, 0, 0, 0, 0);
    }
    func_001C60B8(p, n);
}
