struct E { int n; int a, b, c; };
/** @sly2-annot addr=0x17F3E0
 * Purpose: Set n=-1 on c entries
 * Info: proposed name InitCid47Entries (T2); class CID47; TU tu_17F090
 * Notes: names_v2 v1 proposed SggsGetStepguard__FP9STEPGUARD (0.036)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r4b_rest
 * See: docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
void func_0017F3E0(int x, int c, struct E *pe) { int i; for (i = 0; i < c; i++) { pe->n = -1; pe++; } }
