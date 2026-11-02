void ApplySmCore(int, int, int, int, int, int *);
/** @sly2-annot addr=0x1F03A8
 * Purpose: Wrapper with explicit kind
 * Info: proposed name PsmaApplySmEx (T1); TU sm
 * Confidence: mid (draft) [T1 0.65] - evidence: semantic review; sources: semantic_tu_docs/sm
 * See: docs/reference/functions/animation.md#sm-0x1efa90-0x1f1768
 * @end-sly2-annot */
int PsmaApplySmEx(int a, int b, int c, int d, int e) { int r; ApplySmCore(a, b, c, d, e, &r); return r; }
