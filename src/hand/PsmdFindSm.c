struct E { int fl; int b; };
struct S { char pad[0x60]; int c; struct E *ae; };
/** @sly2-annot addr=0x1F0458
 * Purpose: First distance-table record whose mask intersects grfMask
 * Info: proposed name PsmdFindSm (T1); TU sm
 * Confidence: mid (draft) [T1 0.65] - evidence: semantic review; sources: semantic_tu_docs/sm
 * See: docs/reference/functions/animation.md#sm-0x1efa90-0x1f1768
 * @end-sly2-annot */
struct E *PsmdFindSm(struct S *p, int m)
{
    int i;
    for (i = 0; i < p->c; i++) { struct E *pe = &p->ae[i]; if (pe->fl & m) return pe; }
    return 0;
}
