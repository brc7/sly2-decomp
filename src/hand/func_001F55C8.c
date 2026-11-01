typedef unsigned long ulong;
struct VTE { short d; short pad; void *(*pfn)(void *); };
struct O;
struct Q { char pad[0x60]; struct O *p60; };
struct O { char pad[0xC]; struct VTE *pvt; char pad2[0x28 - 0x10]; ulong fl; char pad3[0xEC - 0x30]; struct Q *pq; };
struct G;
extern struct G D_003DD490;
struct O *func_001F2798(struct O *);
int CploFindSwObjectsByClass(struct G *, int, int, struct O *, int, struct O **);
void func_001F2BA8(struct O *);
void RecalcSwOxfFilterForObject(struct G *, struct O *);
void func_001F2B20(struct O *);
void func_001F5078(struct O *);
void func_001F55C8(struct O *p)
{
    struct O *ap[64];
    int i;
    int n;
    if (!((p->fl >> 60) & 1)) return;
    if (func_001F2798(p) != p->pq->p60) {
        n = CploFindSwObjectsByClass(&D_003DD490, 1, 0x30, p, 0x40, ap);
        for (i = 0; i < n; i++) {
            {
                struct O *po = ap[i];
                if (po->pq) {
                    struct O *q = func_001F2798(po);
                    if (q != po->pq->p60) {
                        func_001F2BA8(po);
                        if (!q->pq) {
                            q->pq = q->pvt[53].pfn((char *)q + q->pvt[53].d);
                            RecalcSwOxfFilterForObject(&D_003DD490, q);
                        }
                        po->pq = q->pq;
                        func_001F2B20(po);
                    }
                }
            }
        }
    }
    RecalcSwOxfFilterForObject(&D_003DD490, p);
    func_001F5078(p);
}
