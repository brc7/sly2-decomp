struct S { char pad[0x2AC]; unsigned int c; int a[4]; };
/** @sly2-annot addr=0x1497E0
 * Purpose: append to a 4-entry list
 * Info: proposed name AddVolbtnList4 (T2); TU button
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/gameplay-objects.md#button-0x148620-0x149a50
 * @end-sly2-annot */
void func_001497E0(struct S *p, int x)
{
    if (p->c < 4) {
        p->a[p->c++] = x;
    }
}
