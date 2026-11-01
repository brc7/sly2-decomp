struct S { char pad[0x30]; int n30; };
void func_001EEF68(struct S *p);
/** @sly2-annot addr=0x17B590
 * Purpose: BHVSHOOT Init: state -1
 * Info: proposed name InitBhvshoot (T1); class BHVSHOOT slot7; TU guardbhv
 * Notes: BHV class word normalised: InitBhvshoot -> InitBhv173 | v3 BHV class word (runtime6 P2: BHV194 =
 *   follow, BHV168 = walk-to-goal child): InitBhv173 -> InitBhvshoot | runtime7a: BHV173 (BHVSHOOT) on live
 *   goat guard stack [156 165 173 157] S67
 * Confidence: mid (draft) [T1 0.60] - evidence: runtime (save-state RAM); sources:
 *   r9_bhv_screen+runtime(class role)+runtime
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_0017B590(struct S *p) { func_001EEF68(p); p->n30 = -1; }
