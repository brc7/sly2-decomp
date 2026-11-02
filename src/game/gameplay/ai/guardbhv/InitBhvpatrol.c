struct S { char pad[0x10]; void *p10; char pad2[0x1C]; float g30; float g34; int n38; };
void InitBhv(struct S *p);
void AddStepguardSgf(void *p, int n);
/** @sly2-annot addr=0x179A58
 * Purpose: BHVPATROL Init: raise flag 0x1E (patrolling), timers off
 * Info: proposed name InitBhvpatrol (T1); class BHVPATROL slot7; TU guardbhv
 * Notes: runtime2/4 confirmed the BHVPATROL role on live guards (S02-S41)
 * Confidence: high [T1 0.80] - evidence: runtime (save-state RAM); sources: r9_bhv_screen+runtime(class role)
 * See: docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void InitBhvpatrol(struct S *p) { InitBhv(p); p->n38 = -1; p->g30 = p->g34 = 3.40282346638528859812e+38f; AddStepguardSgf(p->p10, 0x1E); }
