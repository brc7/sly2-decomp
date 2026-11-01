struct Q { int x0; struct Q *next; int x8; int xc; void *x10; };
struct S { char pad[0x18]; int x18; char pad2[0x20]; struct Q *pq; };
/** @sly2-annot addr=0x1A6290
 * Purpose: True if plo->oid==oid or any proxy-alias node owned by plo has that oid
 * Info: proposed name FMatchesLoName (T1); TU lo
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r4_17F090+verdicts_tu_r4r6
 * See: docs/engine/object-model.md#9-key-functions,
 *   docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518
 * @end-sly2-annot */
int func_001A6290(struct S *p, int b)
{
    struct Q *q;
    if (p->x18 == b) return 1;
    for (q = p->pq; q && q->x10 == p; q = q->next)
        if (q->x8 == b) return 1;
    return 0;
}
