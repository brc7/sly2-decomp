typedef int qword __attribute__((mode(TI)));
struct CLOCK { float t; };
extern struct CLOCK D_002DDED0;
extern qword D_002A2DD0;
struct P { char pad[0x58]; int x58; int x5C; char pad0[0x190 - 0x60]; qword q190; char pad1[0x1030 - 0x1A0]; char x1030[4]; char x1034[4]; char pad2[0x1630 - 0x1038]; char x1630[4]; void *x1634; char pad3[0x16E0 - 0x1638]; int x16E0; char pad4[0x17F0 - 0x16E4]; void *x17F0; int x17F4; float f17F8; };
void func_002303A0(struct P *p, int a, int b);
void func_0020BD68(void *p);
void ReleaseAsega(void *p);
void ReleaseSma(void *p);
void func_001305B8(void *a, struct P *p, int n, void *q, float f1, float f2);
void ReplaceSma(int a, struct P *p, int b, int n, void *q);
void func_001B5D30(struct P *p, int st)
{
    void *o;
    int x, n;
    if (st == p->x17F4) return;
    if (p->x17F4 == 0) {
        func_002303A0(p, p->x58, 2);
        func_002303A0(p, p->x5C, 2);
    }
    p->x17F4 = st;
    p->f17F8 = D_002DDED0.t;
    o = 0;
    x = 0;
    n = 1;
    switch (st) {
    case 0:
        func_002303A0(p, p->x58, 1);
        func_002303A0(p, p->x5C, 1);
        x = p->x16E0;
        p->q190 = D_002A2DD0;
        break;
    case 2:
        o = p->x17F0;
        n = 9;
        break;
    case 1:
        o = p->x1634;
        func_0020BD68(p->x1630);
        n = 9;
        break;
    }
    ReleaseAsega(p->x1030);
    ReleaseSma(p->x1034);
    func_001305B8(o, p, n, p->x1030, 0.0f, 1.0f);
    ReplaceSma(x, p, -1, n, p->x1034);
}
