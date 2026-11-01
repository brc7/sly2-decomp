struct VTE { short d; short pad; void (*pfn)(); };
struct O { int pad[3]; struct VTE *vt; };
struct Q { char pad[0x60]; struct O *po; };
struct S { char pad[0x3d28]; struct Q *pq; };
void FlushSwBoundsUpdates(struct S *p) {
    struct Q *pq;
    while ((pq = p->pq) != 0) {
        struct O *po = pq->po; struct VTE *vt = po->vt;
        ((void (*)(void *))vt[49].pfn)((char *)po + vt[49].d);
    }
}
