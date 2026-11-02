void ApplySmCore(int, int, int, int, int, int);
/** @sly2-annot addr=0x1F0198
 * Purpose: Wrapper: core with kind 4
 * Info: proposed name PsmaApplySmOut (T1); TU sm
 * Confidence: mid (draft) [T1 0.65] - evidence: semantic review; sources: semantic_tu_docs/sm
 * See: docs/reference/functions/animation.md#sm-0x1efa90-0x1f1768,
 *   docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void PsmaApplySmOut(int a, int b, int c, int d, int e) { ApplySmCore(a, b, c, d, 4, e); }
