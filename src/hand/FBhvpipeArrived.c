struct T { char pad[0xF88]; float gF88; };
struct S { char pad[0x10]; struct T *pt; char pad2[0x24]; float g38; };
/** @sly2-annot addr=0x17D188
 * Purpose: |param - goal| < 50
 * Info: proposed name FBhvpipeArrived (T1); TU guardbhv
 * Notes: unverified names_v2 tier-B alignment hint (tier B verified ~33% precise) | superseded:
 *   SetAloRotationSmooth (names_v2:B(unverified))
 * Confidence: mid (draft) [T1 0.50] - evidence: static naming pass; sources:
 *   names_v2:B(unverified)+r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
int FBhvpipeArrived(struct S *p) { return __builtin_fabsf(p->pt->gF88 - p->g38) < 50.0f; }
