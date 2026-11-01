
struct VTE { short d; short pad; void (*pf)(void *); };
struct OW { char pad[0x26C]; int x; };
struct O { char pad[0xC]; struct VTE *vt; struct OW *pow; };
struct S { struct O *po; };
void func_001EEED0(int *p);
/** @sly2-annot addr=0x1EF0D8
 * Purpose: Given a BHV* slot: if non-null call its vt9 (Close), decrement its step's cpbhv, null the slot
 * Info: proposed name PopBhv (T1); TU bhvstk
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r6b_rest+verdicts_followups
 * See: docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/guards-ai.md#bhvstk-0x1eec60-0x1ef850
 * @end-sly2-annot */
void func_001EF0D8(struct S *p)
{
    struct O *po = p->po;
    if (po) {
        int *px = &po->pow->x;
        po->vt[9].pf((char *)po + po->vt[9].d);
        func_001EEED0(px);
        p->po = 0;
    }
}
