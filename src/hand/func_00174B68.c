struct S { char pad[0x30]; int n30; char pad2[0xC]; void *p40; };
void func_001EEF98(struct S *p);
int func_00209430(void *p);
void func_001749C0(struct S *p, int n);
/** @sly2-annot addr=0x174B68
 * Purpose: BHVANIM Update: finish (state 1) when the anim binding is done
 * Info: proposed name UpdateBhvanim (T1); class BHVANIM slot10; TU guardbhv
 * Notes: runtime2/4 confirmed the BHVANIM role on live guards (S02-S41)
 * Confidence: high [T1 0.80] - evidence: runtime (save-state RAM); sources: r9_bhv_screen+runtime(class role)
 * See: docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_00174B68(struct S *p) {
    int s;
    func_001EEF98(p);
    do {
        s = p->n30;
        if (s == 0 && func_00209430(p->p40)) func_001749C0(p, 1);
    } while (s != p->n30);
}
