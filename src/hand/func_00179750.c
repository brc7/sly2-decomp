struct S { char pad[0x3C]; int n3C; };
void func_00174920(struct S *p);
/** @sly2-annot addr=0x179750
 * Purpose: BHVKNOCKDOWN Init: BHVHIT Init with flag 6
 * Info: proposed name InitBhv162 (T2); class BHV162 slot7; TU guardbhv
 * Notes: BHV class word normalised: InitBhvknockdown -> InitBhv162
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_00179750(struct S *p) { func_00174920(p); p->n3C = 6; }
