struct T { char pad[0x180]; int n; };
struct S { char pad[0x13EC]; int f; struct T *pt; char pad2[0x144C - 0x13F4]; int g; };
void func_001B6078(struct S *p, int n);
void func_002046F0(struct S *p, int n);
/** @sly2-annot addr=0x1B7050
 * Purpose: Same as 0x1B7000 with clip kind 1 (alternate lift clip x1444)
 * Info: proposed name StartMurrayLiftGuard (T1); TU murray
 * Notes: runtime4: guard lift path (code; circle takedown not observed)
 * Confidence: high [T1 0.85] - evidence: runtime (save-state RAM); sources:
 *   r5b_rest+runtime+verdicts_followups
 * See: docs/engine/player.md#11-key-functions, docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
void func_001B7050(struct S *p, struct T *pt)
{
    if (p->f) {
        p->pt = pt;
        p->g = 1;
        func_001B6078(p, 4);
        func_002046F0(p, p->pt->n);
    }
}
