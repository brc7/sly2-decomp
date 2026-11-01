struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
struct A { char pad[0x74]; float g74; };
struct B { char pad[0x1C]; float g1C; };
struct E { int n0; struct A *pa; struct B *pb; int nC; };
struct O { char pad[0x1A4]; int n1A4; char pad2[0xFD0 - 0x1A8]; char aFD0[0x20]; float gFF0; float gFF4; };
struct S { char pad[0x10]; struct O *p10; char pad2[0x1C]; int n30; float g34; char pad3[8]; struct E *pe; };
void func_00209788(void *p, struct S *ps);
void func_00209790(void *p, struct S *ps);
int func_00209A68(struct O *o, int n);
void func_00209278(struct E *e, struct O *o, int n, float g);
void func_00209360(struct E *e);
void func_001EF038(struct S *p);
/** @sly2-annot addr=0x1749C0
 * Purpose: BHVANIM state switch: 0 = become mover driver and StartAnimBinding (phase-continue walk cycle when
 *   flag 9), 1 = ClearBhvBusy, save walk phase, StopAnimBinding, release mover
 * Info: proposed name SetBhvanimState (T1); TU guardbhv
 * Notes: runtime2/4 confirmed the BHVANIM role on live guards (S02-S41)
 * Confidence: mid (draft) [T1 0.75] - evidence: runtime (save-state RAM); sources:
 *   r9_bhv_screen+runtime(class role)
 * See: docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_001749C0(struct S *p, int n) {
    if (p->n30 == n) return;
    p->n30 = n;
    p->g34 = D_002DDED0.t;
    switch (n) {
    case 0: {
        float f;
        func_00209788(p->p10->aFD0, p);
        if (p->pe->n0 == 0 && func_00209A68(p->p10, 9) && D_002DDED0.t == p->p10->gFF4)
            f = p->pe->pa->g74 * p->p10->gFF0;
        else
            f = 0.0f;
        func_00209278(p->pe, p->p10, p->pe->nC | 1, f);
        break;
    }
    case 1:
        func_001EF038(p);
        if (p->pe) {
            if (p->pe->n0 == 0 && func_00209A68(p->p10, 9)) {
                p->p10->gFF0 = p->pe->pb->g1C / p->pe->pa->g74;
                p->p10->gFF4 = D_002DDED0.t;
            }
            func_00209360(p->pe);
            p->pe = 0;
            func_00209790(p->p10->aFD0, p);
        }
        p->p10->n1A4 = 0;
        break;
    }
}
