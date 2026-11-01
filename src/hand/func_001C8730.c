struct VTE { short d; short pad; void (*pf)(); };
struct LO { char pad[0xC]; struct VTE *vt; };
struct S {
    char pad[0xC]; struct VTE *vt;
    char p0[0x180 - 0x10]; int x180; float x184; int p188[2]; struct LO *x190; int x194; int p198; void *x19C; int p1A0; int x1A4;
    char p1[0x1F4 - 0x1A8]; void *x1F4; int p1F8; void *x1FC; int x200; int p204[2]; void *a20C[6];
};
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
extern char D_003DD040[];
extern char D_003DD490[];
void func_001FE738(void *p);
void func_001DFEB8(void *p);
void func_001C8918(struct S *p, int n);
void ReleaseSwLetterbox(void *p);
void func_001DAF38(int n, int f);
int func_001FE5F0(void *p, void *q, int f);
void func_0020F198(void *p);
void AcquireSwLetterbox(void *p);
void func_0020F1A8(void *p);
void func_001DFD58(void *p, struct LO *q);
void func_001C8730(struct S *p, int n)
{
    void *pv;
    if (n == p->x180) return;
    if (p->x1A4) {
        func_001FE738(D_003DD040);
        p->x1A4 = 0;
    }
    if (p->x19C)
        func_001DFEB8(p->x19C);
    switch (p->x180) {
    case 2:
        func_001C8918(p, -1);
        ReleaseSwLetterbox(D_003DD490);
        break;
    case 3:
        if (p->x200 != -1)
            func_001DAF38(p->x200, 1);
        if (p->x1FC)
            p->x1A4 = func_001FE5F0(D_003DD040, p->x1FC, 1);
        break;
    case 0:
        if (p->x190)
            func_0020F198(D_003DD490);
        break;
    }
    p->x180 = n;
    p->x184 = D_002DDED0.t;
    switch (n) {
    case -1:
        return;
    case 2:
        p->x194 = 5;
        func_001C8918(p, 1);
        AcquireSwLetterbox(D_003DD490);
        if (p->x1F4)
            p->x1A4 = func_001FE5F0(D_003DD040, p->x1F4, 0);
        break;
    case 1:
        ((void (*)(void *, int))p->vt[79].pf)((char *)p + p->vt[79].d, 1);
        break;
    case 0:
    case 5: {
        struct LO *q = p->x190;
        if (q) {
            ((void (*)(void *, int))q->vt[79].pf)((char *)q + q->vt[79].d, 1);
            p->x190 = 0;
            func_0020F1A8(D_003DD490);
        }
        break;
    }
    }
    pv = p->a20C[p->x180];
    p->x19C = pv;
    if (pv)
        func_001DFD58(pv, p->x190);
}
