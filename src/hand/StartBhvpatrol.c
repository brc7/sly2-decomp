typedef int qword __attribute__((mode(TI)));
struct T { char pad[0x2DC]; int n2DC; char pad2[8]; int n2E8; int n2EC; char pad3[0xFE8 - 0x2F0]; int nFE8; };
struct S { char pad[0x10]; struct T *pt; char pad2[0x2C]; int n40; };
void StartBhv(struct S *p);
void SetBhvpatrolState(struct S *p, int n);
qword func_001A62E8(void *p);
void func_00170E90(int n, qword q, void *pv);
int func_00209098(struct T *t);
/** @sly2-annot addr=0x179AA0
 * Purpose: BHVPATROL Start: take the patrol script (+0x2E8, or +0x2EC with capability 2); resume at nearest
 *   script point; walk (0) or stand (2) or hands-off (3)
 * Info: proposed name StartBhvpatrol (T1); class BHVPATROL slot8; TU guardbhv
 * Notes: runtime2/4 confirmed the BHVPATROL role on live guards (S02-S41)
 * Confidence: high [T1 0.80] - evidence: runtime (save-state RAM); sources: r9_bhv_screen+runtime(class role)
 * See: docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void StartBhvpatrol(struct S *p) {
    struct T *t;
    StartBhv(p);
    t = p->pt;
    if (t->n2E8) p->n40 = t->n2E8;
    else if (t->nFE8 & 2) p->n40 = t->n2EC;
    else { p->n40 = 0; SetBhvpatrolState(p, 2); return; }
    if (p->pt->n2DC) func_00170E90(p->n40, func_001A62E8(p->pt), &p->pt->n2DC);
    if (func_00209098(p->pt)) SetBhvpatrolState(p, 3); else SetBhvpatrolState(p, 0);
}
