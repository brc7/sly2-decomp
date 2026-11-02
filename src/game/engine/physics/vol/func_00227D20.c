typedef int qword __attribute__((mode(TI)));
typedef struct MSG { int cb; int nKind; } MSG;
typedef struct MARK { int a[4]; } MARK;
struct S { char pad[0xE4]; int c; int pad1; void *pEC; };
static __inline__ int Min(int a, int b) { return a < b ? a : b; }
int FCheckVolPoint(struct S *p, qword v, float g);
MARK *STACKSCOPE_ctor(MARK *pm);
void STACKSCOPE_ctor_1AE4A0(MARK *pm, int n);
void *func_001AE318(MSG *pmsg);
qword ConvertAloPos(int a, struct S *p, qword q);
int func_00118A88(void *p, int n, qword v, int *pc, void **ppv, float g);
/** @sly2-annot addr=0x227D20
 * Purpose: radius<=1 -> point test else BSP prune with radius (0x118A88)
 * Info: proposed name FCheckVolSphere (T2); TU vol
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/physics-collision.md#vol-0x227bb8-0x227fe8
 * @end-sly2-annot */
int func_00227D20(struct S *p, qword v, float g)
{
    if (g <= 1.0f) {
        return FCheckVolPoint(p, v, g);
    } else {
        MARK mark;
        int f = 0;
        MSG msg;
        int c;
        void *pv;
        STACKSCOPE_ctor(&mark);
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
        STACKSCOPE_ctor_1AE4A0(&mark, 2);
        return f;
    }
}
