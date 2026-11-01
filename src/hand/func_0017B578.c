struct S { char pad[0x50]; int n50; };
/** @sly2-annot addr=0x17B578
 * Purpose: BHVHELD ground-probe flags: 7 when landed (state 3) else 1
 * Info: proposed name BHVJUGGLED_GrfGroundProbe (T1); class BHVJUGGLED slot12; TU guardbhv
 * Notes: BHV class word normalised: BHVHELD_GrfGroundProbe -> BHVJUGGLED_GrfGroundProbe | runtime2/4
 *   confirmed the BHVJUGGLED role on live guards (S02-S41)
 * Confidence: mid (draft) [T1 0.55] - evidence: runtime (save-state RAM); semantic review; sources:
 *   r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
int func_0017B578(struct S *p) { return p->n50 != 3 ? 1 : 7; }
