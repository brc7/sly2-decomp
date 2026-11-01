typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f; } U;
extern float D_002BE74C;
struct L { int pad[2]; void *p8; };
struct T { char pad[0x40]; struct L *p40; };
struct A { char pad[0xF84]; void *pF84; float fF88; int nF8C; char pad2[0xFD0 - 0xF90]; char aFD0[4]; };
struct S { char pad[0x10]; struct A *p10; int pad2; struct T *p18; char pad3[0x14]; int n30; int pad4; float f38; float f3C; };
union V { qword q; float f[4]; };
void UpdateBhv(struct S *p);
qword func_001A62E8(void *p);
float SWrapShape(void *p, float f);
void FindShapeClosestPointFromS(void *p, float f, qword v, int a, int b, union V *pv, int c, float *pt);
int FBhvrailArrived(struct S *p);
void SetBhvrailState(struct S *p, int n);
float func_002458F0(float y, float x);
void SetStepguardMoverRadTarget(void *p, float f);
void func_00131A00(void *p, float f);
/** @sly2-annot addr=0x17CB80
 * Purpose: BHVRAIL Update: walk along the attached rail shape (+0xF84, param +0xF88) toward the goal
 *   parameter; stand on arrival
 * Info: proposed name UpdateBhvrail (T1); class BHVRAIL slot10; TU guardbhv
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void UpdateBhvrail(struct S *p) {
    int s;
    UpdateBhv(p);
    do {
        s = p->n30;
        if (s == 0) {
            struct A *a = p->p10;
            void *q = a->pF84;
            float t = a->fF88;
            qword pos;
            union V v;
            a->nF8C = t < p->f3C;
            pos = func_001A62E8(p->p10);
            FindShapeClosestPointFromS(q, SWrapShape(q, p->p10->nF8C ? t + D_002BE74C : t - D_002BE74C), pos, 0, 0, &v, 0, &t);
            p->p10->fF88 = t;
            if (FBhvrailArrived(p)) {
                SetBhvrailState(p, 1);
            } else {
                if (p->p10->nF8C == 0) {
                    qword w = v.q;
                    U u;
                    __asm__("qmtc2.ni %1, %0" : "=j"(u.q) : "r"(-1.0f));
                    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(v.q) : "j"(w), "j"(u.q));
                }
                SetStepguardMoverRadTarget(p->p10->aFD0, func_002458F0(v.f[1], v.f[0]));
                func_00131A00(p->p18->p40->p8, p->f38);
            }
        }
    } while (p->n30 != s);
}
