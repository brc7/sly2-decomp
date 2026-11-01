struct S { char pad[0x30]; int n30; };
void func_001EEF68(struct S *p);
/** @sly2-annot addr=0x1766D8
 * Purpose: BHVGUARD Init: state kind -1
 * Info: proposed name InitBhvguard (T1); class BHVGUARD slot7; TU guardbhv
 * Notes: runtime2/4 confirmed the BHVGUARD role on live guards (S02-S41)
 * Confidence: mid (draft) [T1 0.75] - evidence: runtime (save-state RAM); sources:
 *   r9_bhv_screen+runtime(class role)
 * See: docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_001766D8(struct S *p) { func_001EEF68(p); p->n30 = -1; }
