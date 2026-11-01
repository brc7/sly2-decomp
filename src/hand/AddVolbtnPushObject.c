struct S { char pad[0x288]; unsigned int c; int a[8]; };
/** @sly2-annot addr=0x1497B8
 * Purpose: append to push list (max 8)
 * Info: proposed name AddVolbtnPushObject (T1); TU button
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/gameplay-objects.md#button-0x148620-0x149a50
 * @end-sly2-annot */
void AddVolbtnPushObject(struct S *p, int x)
{
    if (p->c < 8) {
        p->a[p->c++] = x;
    }
}
