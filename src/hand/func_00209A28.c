/** @sly2-annot addr=0x209A28
 * Purpose: popcount(a) if a is a subset of b, else 0
 * Info: proposed name CsgfMatch (T2); TU stepguard
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
int func_00209A28(unsigned long a, unsigned long b) {
    unsigned long x = a & b;
    int c;
    if (x != a) return 0;
    for (c = 0; x != 0; c++) x &= x - 1;
    return c;
}
