struct S { char pad[0x3C]; int n3C; };
void InitBhvanim(struct S *p);
/** @sly2-annot addr=0x1796A0
 * Purpose: BHVHURT Init: BHVHIT Init with flag 5 (damaged)
 * Info: proposed name InitBhv161 (T2); class BHV161 slot7; TU guardbhv
 * Notes: BHV class word normalised: InitBhvhurt -> InitBhv161
 * Confidence: low (draft) [T2 0.50] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_001796A0(struct S *p) { InitBhvanim(p); p->n3C = 5; }
