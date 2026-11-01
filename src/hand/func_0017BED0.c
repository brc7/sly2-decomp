struct S { char pad[0x10]; void *p10; char pad2[0x1C]; int n30; };
void func_001EEF68(struct S *p);
void func_00209890(void *p, int n);
/** @sly2-annot addr=0x17BED0
 * Purpose: BHVCIRCLE Init: raise flag 0x19, state -1
 * Info: proposed name InitBhv179 (T2); class BHV179 slot7; TU guardbhv
 * Notes: BHV class word normalised: InitBhvcircle -> InitBhv179
 * Confidence: low (draft) [T2 0.85] - evidence: static analysis; sources: r9_bhv_screen+verdicts_followups
 * See: docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_0017BED0(struct S *p) { func_001EEF68(p); func_00209890(p->p10, 0x19); p->n30 = -1; }
