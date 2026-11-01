struct VTE { short d; short pad; void (*pfn)(void *); };
struct LO { char pad[0xC]; struct VTE *vt; };
struct X14 { struct LO *plo; };
struct P { char pad[0xC]; struct VTE *vt; char pad1[0x14 - 0x10]; struct X14 *px14; char pad2[0x150 - 0x18]; int state; float tState; char pad3[0x160 - 0x158]; void *p160; };
struct CLOCK { char pad[0x3C]; float t3C; };
extern struct CLOCK D_002DDED0;
extern struct P *D_002DD644;
extern char D_003DD040[];
extern char D_003DD490[];
void func_001FE5B8(void *p, int a, int b);
void func_0014D1D0(struct CLOCK *p, int a, float g);
void func_0020EFC0(void *p, int a);
void func_0020F030(void *p, int a);
int func_001A61A0(struct P *p);
void AppendDlEntry__FP2DLPv(void *p, struct X14 *px);
void func_00131A20(void *p, float g);
void func_00147410(struct P *p, int state)
{
    if (state == p->state) return;
    switch (p->state) {
    case -1:
        D_002DD644 = p;
        func_001FE5B8(D_003DD040, 2, 0);
        func_0014D1D0(&D_002DDED0, 8, 0.0f);
        func_0020EFC0(D_003DD490, 0);
        p->vt[8].pfn((char *)p + p->vt[8].d);
        if (!func_001A61A0(p)) {
            struct LO *plo;
            AppendDlEntry__FP2DLPv(D_003DD490 + 0x2C, p->px14);
            plo = p->px14->plo;
            plo->vt[23].pfn((char *)plo + plo->vt[23].d);
        }
        break;
    case 3:
        if (p->p160) func_00131A20(p->p160, 1.0f);
        break;
    }
    p->state = state;
    p->tState = D_002DDED0.t3C;
    switch (state) {
    case -1:
        D_002DD644 = 0;
        func_0014D1D0(&D_002DDED0, 8, 1.0f);
        func_001FE5B8(D_003DD040, 2, 1);
        func_0020F030(D_003DD490, 0);
        p->vt[9].pfn((char *)p + p->vt[9].d);
        break;
    case 3:
        if (p->p160) func_00131A20(p->p160, 0.0f);
        break;
    }
}
