typedef int qword __attribute__((mode(TI)));
typedef struct MSG { int cb; int nKind; } MSG;
typedef struct MARK { int a[4]; } MARK;
struct S { char pad[0xE4]; int c; int pad1; void *pEC; };
static __inline__ int Min(int a, int b) { return a < b ? a : b; }
int func_00227C90(struct S *p, qword v, float g);
MARK *func_001AE478(MARK *pm);
void func_001AE4A0(MARK *pm, int n);
void *func_001AE318(MSG *pmsg);
qword ConvertAloPos(int a, struct S *p, qword q);
int func_00118A88(void *p, int n, qword v, int *pc, void **ppv, float g);
int func_00227D20(struct S *p, qword v, float g)
{
    if (g <= 1.0f) {
        return func_00227C90(p, v, g);
    } else {
        MARK mark;
        int f = 0;
        MSG msg;
        int c;
        void *pv;
        func_001AE478(&mark);
        {
            int cT = p->c;
            MSG *pm = &msg;
            cT = Min(cT, 0x1555);
            c = cT;
            pm->cb = cT * 12;
            pm->nKind = 0x10;
            pv = func_001AE318(pm);
        }
        {
            int r = func_00118A88(p->pEC, 0x7FFFFFFF, ConvertAloPos(0, p, v), &c, &pv, g);
            if (r == 0 || r == 2) f = 1;
        }
        func_001AE4A0(&mark, 2);
        return f;
    }
}
