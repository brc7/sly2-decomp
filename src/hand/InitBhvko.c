struct S { char pad[0x3C]; int n3C; };
void InitBhvanim(struct S *p);
/** @sly2-annot addr=0x179868
 * Purpose: BHVKO Init: BHVHIT Init with flag 7 (dead/KO)
 * Info: proposed name InitBhvko (T1); class BHVKO slot7; TU guardbhv
 * Notes: runtime2/4 confirmed the BHVKO role on live guards (S02-S41)
 * Confidence: mid (draft) [T1 0.70] - evidence: runtime (save-state RAM); sources:
 *   r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void InitBhvko(struct S *p) { InitBhvanim(p); p->n3C = 7; }
