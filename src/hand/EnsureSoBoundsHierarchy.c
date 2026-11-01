struct L { char pad[0x24]; struct L *next; char pad2[0x64 - 0x28]; struct L *first; char pad3[0x70 - 0x68]; struct L *end; };
void EnsureSoBounds(struct L *);
int FIsBasicDerivedFrom__FP5BASIC3CID(struct L *, int);
/** @sly2-annot addr=0x1F26A8
 * Purpose: EnsureSoBounds on this SO and recursively on SO children
 * Info: proposed name EnsureSoBoundsHierarchy (T1); TU so
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void EnsureSoBoundsHierarchy(struct L *p)
{
    struct L *q;
    EnsureSoBounds(p);
    for (q = p->first; q != p->end; q = q->next) {
        if (FIsBasicDerivedFrom__FP5BASIC3CID(q, 0x30)) EnsureSoBoundsHierarchy(q);
    }
}
