struct S { char pad[0x3C]; int n3C; };
void func_00174920(struct S *p);
/** @sly2-annot addr=0x174FC8
 * Purpose: BHVIDLE Init: BHVANIM Init with default flag 2 (stand/idle)
 * Info: proposed name InitBhvidle (T1); class BHVIDLE slot7; TU guardbhv
 * Notes: runtime2: BHV158 Init sets +0x3C=2 (observed in every idle stack) | runtime2/4 confirmed the BHVIDLE
 *   role on live guards (S02-S41)
 * Confidence: mid (draft) [T1 0.70] - evidence: runtime (save-state RAM); sources:
 *   r9_bhv_screen+runtime+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_00174FC8(struct S *p) { func_00174920(p); p->n3C = 2; }
