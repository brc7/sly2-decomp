void func_001F01B8(int, int, int, int, int, int *);
/** @sly2-annot addr=0x1F0380
 * Purpose: Wrapper returning the SMA (kind 4)
 * Info: proposed name PsmaApplySm (T1); TU sm
 * Confidence: high [T1 0.85] - evidence: semantic review; sources: semantic_tu_docs/sm
 * See: docs/engine/animation.md#7-key-functions, docs/reference/functions/animation.md#sm-0x1efa90-0x1f1768
 * @end-sly2-annot */
int func_001F0380(int a, int b, int c, int d) { int r; func_001F01B8(a, b, c, d, 4, &r); return r; }
