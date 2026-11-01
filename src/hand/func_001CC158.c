struct S { char pad[0x90]; int a; int x[4]; int b; };
/** @sly2-annot addr=0x1CC158
 * Purpose: JTHANDS: target ids (+0x90, +0xA4) = -1
 * Info: proposed name ResetJtHands (T2); TU jthands
 * Notes: names_v2 C proposed ClearCmFadeObjects__FP2CM (0.200)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r5b_rest
 * See: docs/engine/guards-ai.md#8-source-files, docs/engine/player.md#10-source-files,
 *   docs/reference/functions/player.md#jthands-0x1cc158-0x1cd5f0
 * @end-sly2-annot */
void func_001CC158(struct S *p) { p->b = -1; p->a = -1; }
