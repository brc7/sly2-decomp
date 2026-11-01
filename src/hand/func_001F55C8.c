typedef unsigned long ulong;
struct VTE { short d; short pad; void *(*pfn)(void *); };
struct O;
struct Q { char pad[0x60]; struct O *p60; };
struct O { char pad[0xC]; struct VTE *pvt; char pad2[0x28 - 0x10]; ulong fl; char pad3[0xEC - 0x30]; struct Q *pq; };
struct G;
extern struct G D_003DD490;
struct O *PsoFindPhysRoot(struct O *);
int CploFindSwObjectsByClass(struct G *, int, int, struct O *, int, struct O **);
void RemoveSoParts(struct O *);
void RecalcSwOxfFilterForObject(struct G *, struct O *);
void AddSoParts(struct O *);
void func_001F5078(struct O *);
/** @sly2-annot addr=0x1F55C8
 * Purpose: After physics-root change: regroup contact trees of SOs whose phys root changed, re-register
 * Info: proposed name RebuildSoPhysHook (T2); TU so
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F55C8(struct O *p)
{
    struct O *ap[64];
    int i;
    int n;
    if (!((p->fl >> 60) & 1)) return;
    if (PsoFindPhysRoot(p) != p->pq->p60) {
        n = CploFindSwObjectsByClass(&D_003DD490, 1, 0x30, p, 0x40, ap);
        for (i = 0; i < n; i++) {
            {
                struct O *po = ap[i];
                if (po->pq) {
                    struct O *q = PsoFindPhysRoot(po);
                    if (q != po->pq->p60) {
                        RemoveSoParts(po);
                        if (!q->pq) {
                            q->pq = q->pvt[53].pfn((char *)q + q->pvt[53].d);
                            RecalcSwOxfFilterForObject(&D_003DD490, q);
                        }
                        po->pq = q->pq;
                        AddSoParts(po);
                    }
                }
            }
        }
    }
    RecalcSwOxfFilterForObject(&D_003DD490, p);
    func_001F5078(p);
}
