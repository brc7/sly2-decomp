unsigned long GrfFromSgf(int n);
/** @sly2-annot addr=0x2099F0
 * Purpose: (grf & bit n) != 0
 * Info: proposed name FGrfHasSgf (T1); TU stepguard
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
int FGrfHasSgf(unsigned long fl, int n) { return (fl & GrfFromSgf(n)) != 0; }
