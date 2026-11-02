struct X { char pad[0x54]; int n; char pad2[0x6C - 0x58]; int f6c; };
struct S { char pad[0x94]; int c; struct X **apx; };
/** @sly2-annot addr=0x1645D0
 * Purpose: True if every prerequisite is complete (>=2) or available-and-optional
 * Info: proposed name FTaskPrereqsMet (T1); TU game
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
int FTaskPrereqsMet(struct S *p)
{
    int f = 1;
    int i;
    for (i = 0; f && i < p->c; i++) {
        struct X *px = p->apx[i];
        f = 0;
        if (px->n >= 2 || (px->n == 1 && px->f6c))
            f = 1;
    }
    return f;
}
