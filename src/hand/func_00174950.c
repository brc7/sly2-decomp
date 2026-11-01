struct S { char pad[0x10]; void *p10; char pad2[0x28]; int n3C; int n40; };
void func_001EEF70(struct S *p);
void func_00209890(void *p, int n);
int func_00209BC0(void *p);
void func_001749C0(struct S *p, int n);
/** @sly2-annot addr=0x174950
 * Purpose: BHVANIM Start: raise guard flag +0x3C (AddStepguardSgf), pick anim binding for the current flags
 *   (PanimbindCur), enter state 0 (playing) or 1 (none -> done)
 * Info: proposed name StartBhvanim (T1); class BHVANIM slot8; TU guardbhv
 * Notes: runtime2/4 confirmed the BHVANIM role on live guards (S02-S41)
 * Confidence: high [T1 0.80] - evidence: runtime (save-state RAM); sources: r9_bhv_screen+runtime(class role)
 * See: docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_00174950(struct S *p) {
    func_001EEF70(p);
    func_00209890(p->p10, p->n3C);
    if (!p->n40) {
        p->n40 = func_00209BC0(p->p10);
        if (!p->n40) { func_001749C0(p, 1); return; }
    }
    func_001749C0(p, 0);
}
