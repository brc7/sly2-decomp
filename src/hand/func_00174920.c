struct S { char pad[0x30]; int n30; int pad2[2]; int n3C; };
void func_001EEF68(struct S *p);
/** @sly2-annot addr=0x174920
 * Purpose: BHVANIM Init: base Init, state=-1, flag(+0x3C)=0 (shared by BHV160)
 * Info: proposed name InitBhvanim (T1); class BHVANIM slot7; TU guardbhv
 * Notes: runtime2/4 confirmed the BHVANIM role on live guards (S02-S41)
 * Confidence: high [T1 0.80] - evidence: runtime (save-state RAM); sources: r9_bhv_screen+runtime(class role)
 * See: docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_00174920(struct S *p) { func_001EEF68(p); p->n3C = 0; p->n30 = -1; }
