typedef unsigned long ulong;
struct S { int a[4]; ulong f; };
/** @sly2-annot addr=0x229D60
 * Purpose: returns !((q->grf>>50)&1)
 * Info: TU water
 * Notes: descriptive only: water predicate: !(grf bit50) | no usable identifier; purpose only
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r8_21BB88+verdicts_tu_r7r8
 * @end-sly2-annot */
int func_00229D60(void *p, struct S *q)
{
    return !((q->f >> 50) & 1);
}
