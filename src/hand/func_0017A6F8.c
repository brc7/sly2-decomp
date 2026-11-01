struct S { char pad[0x10]; void *p10; char pad2[0x1C]; int n30; };
void InitBhv(struct S *p);
void AddStepguardSgf(void *p, int n);
/** @sly2-annot addr=0x17A6F8
 * Purpose: BHVTHROWN Init: raise flag 0x16 (thrown), state -1
 * Info: proposed name InitBhv176 (T2); class BHV176 slot7; TU guardbhv
 * Notes: BHV class word normalised: InitBhvthrown -> InitBhv176
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_0017A6F8(struct S *p) { InitBhv(p); AddStepguardSgf(p->p10, 0x16); p->n30 = -1; }
