struct S { char pad[0x2D0]; unsigned int c; int a[8]; };
/** @sly2-annot addr=0x149248
 * Purpose: append to second list (max 8)
 * Info: proposed name AddButtonPushClass (T2); TU button
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/gameplay-objects.md#button-0x148620-0x149a50
 * @end-sly2-annot */
void func_00149248(struct S *p, int x)
{
    if (p->c < 8) {
        p->a[p->c++] = x;
    }
}
