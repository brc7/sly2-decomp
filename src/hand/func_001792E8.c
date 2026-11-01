struct S { char pad[0x4C]; int n4C; char pad2[0x90]; float gE0; };
/** @sly2-annot addr=0x1792E8
 * Purpose: Walking and the stuck timer +0xE0 ran out
 * Info: proposed name FBhvwalkpathStuck (T1); TU guardbhv
 * Notes: runtime2/4 confirmed the BHVWALKPATH role on live guards (S02-S41)
 * Confidence: mid (draft) [T1 0.55] - evidence: runtime (save-state RAM); sources:
 *   r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
int func_001792E8(struct S *p) { int f = 0; if (p->n4C < 2) { if (p->n4C >= 0) f = p->gE0 < 0.0f; } return f; }
