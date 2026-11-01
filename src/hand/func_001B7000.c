struct T { char pad[0x180]; int n; };
struct S { char pad[0x13EC]; int f; struct T *pt; char pad2[0x144C - 0x13F4]; int g; };
void func_001B6078(struct S *p, int n);
void func_002046F0(struct S *p, int n);
/** @sly2-annot addr=0x1B7000
 * Purpose: If lift allowed (x13EC): carried target = grabspot, clip kind 0, SetMurrayState(4), face it
 * Info: proposed name StartMurrayLiftGrabspot (T1); TU murray
 * Notes: runtime4: grabspot lift path (S41) | runtime4: x13EC is an SO pointer (Murray child oid 0x1FF) used
 *   only as a non-NULL lift gate
 * Confidence: mid (draft) [T1 0.55] - evidence: runtime (save-state RAM); sources:
 *   r5b_rest+runtime+runtime-corrected
 * See: docs/engine/player.md#9-grabbable-world-objects, docs/engine/player.md#11-key-functions,
 *   docs/reference/functions/player.md#grabspot-0x182c70-0x1848a8
 * @end-sly2-annot */
void func_001B7000(struct S *p, struct T *pt)
{
    if (p->f) {
        p->pt = pt;
        p->g = 0;
        func_001B6078(p, 4);
        func_002046F0(p, p->pt->n);
    }
}
