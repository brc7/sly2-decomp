typedef int qword __attribute__((mode(TI)));
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
struct X { qword a; qword v; };
struct A { char pad[0xFD0]; char aFD0[4]; };
struct S { char pad[0x10]; struct A *p10; int pad2; void *p18; char pad3[0x14]; int n30; float f34; float f38; float f3C; char pad4[8]; int n48; };
void UpdateBhv(struct S *p);
int FBhvBusyAbove(struct S *p);
int FBhvanimDone(void *p);
int func_00209098(void *p);
int func_00172658(struct S *p);
qword func_00172760(struct S *p);
qword func_001A62E8(void *p);
void func_002562A8(struct X *x);
void SetBhvgotoGoal(void *p, struct X *x);
int FBhvgotoArrived(void *p);
int FBhvgotoPathFinished(void *p);
void func_00172320(struct S *p, int n);
void func_00172560(struct S *p);
void func_00172F10(struct S *p);
float func_002458F0(float y, float x);
void SetStepguardMoverRadTarget(void *p, float f);
static __inline__ float GetX(qword q) { float x; __asm__("mtc1 %1,%0" : "=f"(x) : "r"(q)); return x; }
static __inline__ float GetY(qword q) { float y; __asm__("prot3w %0,%1" : "=r"(y) : "r"(q)); return y; }
/** @sly2-annot addr=0x172018
 * Purpose: Search state machine: 0 react, 1 go to last-known position, 2 face it, 3 visit search points, 4
 *   give up, 5 hold while 0x209098
 * Info: proposed name UpdateBhv171 (T2); class BHV171 slot10; TU tu_171F58
 * Notes: names_v2 C proposed CalculateAloTransform__FP3ALOT0iP6VECTORP7MATRIX3T3T3 (0.438)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r3b_rest
 * See: docs/reference/functions/guards-ai.md#tu_171f58-0x171f58-0x173158
 * @end-sly2-annot */
void func_00172018(struct S *p) {
    int s;
    UpdateBhv(p);
    while (!FBhvBusyAbove(p)) {
        s = p->n30;
        switch (s) {
        case 0:
            if (FBhvanimDone(p->p18)) func_00172320(p, 1);
            break;
        case 1:
            if (func_00209098(p->p10)) {
                func_00172320(p, 5);
            } else if (p->n48) {
                if (func_00172658(p)) {
                    func_00172320(p, 2);
                } else {
                    qword v = func_00172760(p);
                    struct X x;
                    func_002562A8(&x);
                    x.v = v;
                    SetBhvgotoGoal(p->p18, &x);
                }
                p->n48 = 0;
            } else if (func_00172658(p) || FBhvgotoArrived(p->p18) || FBhvgotoPathFinished(p->p18)) {
                func_00172320(p, 2);
            }
            break;
        case 2:
            if (func_00209098(p->p10)) {
                func_00172320(p, 5);
            } else if (p->n48) {
                if (func_00172658(p)) {
                    func_00172560(p);
                } else {
                    func_00172320(p, 1);
                }
                p->n48 = 0;
            } else if (p->f38 < D_002DDED0.t || FBhvanimDone(p->p18)) {
                func_00172320(p, 4);
            } else if (D_002DDED0.t > p->f3C) {
                func_00172320(p, 3);
            } else {
                qword a = func_00172760(p);
                qword b = func_001A62E8(p->p10);
                qword d;
                __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b));
                SetStepguardMoverRadTarget(p->p10->aFD0, func_002458F0(GetY(d), GetX(d)));
            }
            break;
        case 3:
            if (func_00209098(p->p10)) {
                func_00172320(p, 5);
            } else if (p->n48) {
                if (func_00172658(p)) {
                    func_00172320(p, 2);
                } else {
                    func_00172320(p, 1);
                }
                p->n48 = 0;
            } else if (p->f38 < D_002DDED0.t) {
                func_00172320(p, 4);
            } else {
                func_00172F10(p);
            }
            break;
        case 5:
            if (!func_00209098(p->p10)) func_00172320(p, 1);
            break;
        case 4:
            if (p->n48) {
                if (func_00172658(p)) {
                    func_00172320(p, 2);
                } else {
                    func_00172320(p, 1);
                }
                p->n48 = 0;
            } else if (FBhvanimDone(p->p18)) {
                func_00172320(p, -1);
            }
            break;
        }
        if (p->n30 != s) continue;
        break;
    }
}
