unsigned long GrfFromSgf(int n);
/** @sly2-annot addr=0x207A40
 * Purpose: OR of GrfFromSgf over an array
 * Info: proposed name GrfFromSgfArray (T1); TU stepguard
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
unsigned long GrfFromSgfArray(int c, int *a) { unsigned long m = 0; int i; for (i = 0; i < c; i++) m |= GrfFromSgf(a[i]); return m; }
