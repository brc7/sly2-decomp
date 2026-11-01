struct VTE { short d; short pad; void (*pf)(); };
struct CLOCK { float t, dt; };
extern struct CLOCK D_002DDED0;
extern char D_003DD490[];
struct S { char pad[0xC]; struct VTE *vt; char p1[0x1C8 - 0x10]; int x1C8; float x1CC; };
int func_001C4BD8(void *pg, int c, void **ap);
int FIsBasicDerivedFrom__FP5BASIC3CID(struct S *p, int n);
int func_001C5B20(struct S *p, void *pv);
void func_001C5D18(struct S *p)
{
    void *ap[0x10];
    int i, n;
    if (!p->x1C8) return;
    if (p->x1CC < D_002DDED0.t) {
        ((void (*)(void *, int))p->vt[76].pf)((char *)p + p->vt[76].d, 1);
        return;
    }
    n = func_001C4BD8(D_003DD490, 0x10, ap);
    for (i = 0; ; i++) {
        if (i >= n) break;
        if (i > 0 && !FIsBasicDerivedFrom__FP5BASIC3CID(p, 0x5B)) break;
        if (func_001C5B20(p, ap[i])) break;
    }
}
