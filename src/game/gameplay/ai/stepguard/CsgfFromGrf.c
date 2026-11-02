unsigned long GrfFromSgf(int n);
/** @sly2-annot addr=0x207AA0
 * Purpose: List the set flag numbers (46..0) of a mask into an array, return count
 * Info: proposed name CsgfFromGrf (T1); TU stepguard
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
int CsgfFromGrf(unsigned long m, int *a) { int c = 0; int i; for (i = 0x2E; i >= 0; i--) if (m & GrfFromSgf(i)) { *a++ = i; c++; } return c; }
