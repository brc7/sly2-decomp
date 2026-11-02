typedef int qword __attribute__((mode(TI)));
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
extern float D_002BE758, D_002BE75C;
struct W { int pad[0x28 / 4]; int n28; };
struct G { char pad[0x45CC]; struct W *p45CC; };
extern struct G D_003DD490;
struct T { char pad[0x30]; void *p30; float f34; };
struct V { int pad[2]; float f8; };
struct A { char pad[0x218]; struct V *p218; char pad2[0xFE8 - 0x21C]; int nFE8; };
struct S {
    char pad[0x10]; struct A *p10; int pad2; struct T *p18; char pad3[0x14]; int n30;
    char pad4[0x80 - 0x34]; int n80; int n84; char pad5[8]; float a90[4]; float fA0; int nA4;
};
void UpdateBhv(struct S *p);
int FBhvBusyAbove(struct S *p);
int FBhvpathAtEnd(struct T *t);
void SetBhvgotoState(struct S *p, int n);
int FBhvwalkpathStuck(struct T *t);
float SMaxShape(void *p);
void EvaluateShapeS(void *p, float f, float *pv, int n);
int FBhvpathlinkDone(struct T *t);
int func_0017EAE8(struct T *t);
void PlanBhvgoto(struct S *p);
/** @sly2-annot addr=0x17D6C8
 * Purpose: BHVGOTO Update: re-plan when stale or the path graph changed; at a pathlink start traverse it;
 *   stuck -> relocate
 * Info: proposed name UpdateBhvgoto (T1); class BHVGOTO slot10; TU guardbhv
 * Notes: runtime2/4 confirmed the BHVGOTO role on live guards (S02-S41)
 * Confidence: high [T1 0.80] - evidence: runtime (save-state RAM); sources: r9_bhv_screen+runtime(class role)
 * See: docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void UpdateBhvgoto(struct S *p) {
    int s;
    UpdateBhv(p);
    if (!(p->p10->nFE8 & 2)) return;
    while (!FBhvBusyAbove(p)) {
        s = p->n30;
        switch (s) {
        case 1:
        case 2:
            {
                struct T *t = p->p18;
                if (s == 2 && FBhvpathAtEnd(t)) {
                    SetBhvgotoState(p, 3);
                } else if (D_002DDED0.t - p->fA0 > D_002BE758 || p->nA4 != D_003DD490.p45CC->n28) {
                    PlanBhvgoto(p);
                } else if (FBhvwalkpathStuck(t)) {
                    float f = t->f34 + D_002BE75C;
                    float hi = SMaxShape(t->p30);
                    p->n80 = 0;
                    p->n84 = 0;
                    EvaluateShapeS(t->p30, f < hi ? f : hi, p->a90, 0);
                    p->a90[2] = p->p10->p218->f8;
                    SetBhvgotoState(p, 5);
                }
            }
            break;
        case 3:
            if (FBhvpathlinkDone(p->p18)) PlanBhvgoto(p);
            break;
        case 5:
            if (func_0017EAE8(p->p18)) PlanBhvgoto(p);
            break;
        }
        if (s != p->n30) continue;
        break;
    }
}
