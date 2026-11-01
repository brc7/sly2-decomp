struct Q { int a; char *ab; };
struct A { char pad[0x50]; int c; };
struct Q *func_001F0458(struct A *p, int n);
/** @sly2-annot addr=0x1F0498
 * Purpose: table->ab[from?csms + to]: hop count between states under a mask
 * Info: proposed name CsmtDistanceSm (T1); TU sm
 * Confidence: mid (draft) [T1 0.65] - evidence: semantic review; sources: semantic_tu_docs/sm
 * See: docs/reference/functions/animation.md#sm-0x1efa90-0x1f1768
 * @end-sly2-annot */
int func_001F0498(struct A *p, int n, int y, int x)
{
    struct Q *pq = func_001F0458(p, n);
    int r = 0;
    if (pq)
        r = pq->ab[y * p->c + x];
    return r;
}
