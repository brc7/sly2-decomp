struct VTE { short d; short pad; void (*pfn)(); };
struct O { int pad[3]; struct VTE *vt; };
struct Q { char pad[0x60]; struct O *po; };
struct S { char pad[0x3d28]; struct Q *pq; };
/** @sly2-annot addr=0x133E30
 * Purpose: While a queue is non-empty call vt49 (UpdateSoTreeBounds) on its head object
 * Info: proposed name FlushSwBoundsUpdates (T1); TU bbmark
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/physics-collision.md#bbmark-0x1336f0-0x1344f0
 * @end-sly2-annot */
void FlushSwBoundsUpdates(struct S *p) {
    struct Q *pq;
    while ((pq = p->pq) != 0) {
        struct O *po = pq->po; struct VTE *vt = po->vt;
        ((void (*)(void *))vt[49].pfn)((char *)po + vt[49].d);
    }
}
