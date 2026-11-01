struct S { char pad[0x10]; void *p10; char pad2[0x1C]; int n30; };
void func_001EEF68(struct S *p);
void func_00209890(void *p, int n);
/** @sly2-annot addr=0x17DD70
 * Purpose: BHVSTALK Init: raise flag 0x2B, state -1
 * Info: proposed name InitBhv189 (T2); class BHV189 slot7; TU guardbhv
 * Notes: unverified names_v2 tier-B alignment hint (tier B verified ~33% precise) | superseded:
 *   GetAloLookAtPanLimits (names_v2:B(unverified)) | BHV class word normalised: InitBhvstalk -> InitBhv189
 * Confidence: low (draft) [T2 0.85] - evidence: static naming pass; sources:
 *   names_v2:B(unverified)+r9_bhv_screen+verdicts_followups
 * See: docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_0017DD70(struct S *p) { func_001EEF68(p); func_00209890(p->p10, 0x2B); p->n30 = -1; }
