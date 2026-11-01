typedef int qword __attribute__((mode(TI)));
struct Q2 { qword a, b; };
struct S { char pad[0x40]; struct Q2 q; char pad2[0x40]; float gA0; };
void PlanBhvgoto(struct S *p);
/** @sly2-annot addr=0x17DD38
 * Purpose: Set the goal (16-byte header + position) and re-plan
 * Info: proposed name SetBhvgotoGoal (T1); TU guardbhv
 * Notes: unverified names_v2 tier-B alignment hint (tier B verified ~33% precise) | superseded:
 *   SetAloLookAtPanLimits (names_v2:B(unverified)) | runtime2/4 confirmed the BHVGOTO role on live guards
 *   (S02-S41)
 * Confidence: mid (draft) [T1 0.70] - evidence: runtime (save-state RAM); static naming pass; sources:
 *   names_v2:B(unverified)+r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void SetBhvgotoGoal(struct S *p, struct Q2 *pq) { p->q = *pq; p->gA0 = -3.40282346638528859812e+38f; PlanBhvgoto(p); }
