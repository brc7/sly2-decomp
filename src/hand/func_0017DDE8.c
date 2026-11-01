struct T { char pad[0xF64]; void *pF64; };
struct S { char pad[0x10]; struct T *pt; };
void func_001EEF78(struct S *p);
void func_0017E278(struct S *p, int n);
void func_00153FF8(void *p);
void func_00208540(struct T *t, int n);
void func_00209970(struct T *t, int n);
/** @sly2-annot addr=0x17DDE8
 * Purpose: BHVSTALK Close: state -1, reset detector, clear target, lower flag 0x2B
 * Info: proposed name CloseBhv189 (T2); class BHV189 slot9; TU guardbhv
 * Notes: unverified names_v2 tier-B alignment hint (tier B verified ~33% precise) | superseded:
 *   SetAloLookAtTiltLimits (names_v2:B(unverified)) | BHV class word normalised: CloseBhvstalk -> CloseBhv189
 * Confidence: low (draft) [T2 0.35] - evidence: static naming pass; sources:
 *   names_v2:B(unverified)+r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_0017DDE8(struct S *p) { func_001EEF78(p); func_0017E278(p, -1); func_00153FF8(p->pt->pF64); func_00208540(p->pt, 0); func_00209970(p->pt, 0x2B); }
