typedef int ti128 __attribute__((mode(TI)));
struct VTE { short d; short pad; void (*pfn)(); };
struct E { char pad[0x18]; int f18; };
struct O { int pad[3]; struct VTE *vt; char pad2[0xe4 - 0x10]; struct E *pe; };
struct Q { char pad[0x50]; ti128 v50; int f60; };
struct S { char pad[0x16d8]; int f16d8; void *f16dc; void *f16e0; struct Q *pq; ti128 v16f0; float f1700; };
extern ti128 D_002A2DD0;
extern float D_002DDED0;
void func_001356E8(struct S *);
struct O *PloAcquireProxy(void *, int);
void LaunchSoFromLauncher(struct Q *, struct O *);
void PrimeBomb__FP4BOMBf(struct O *, float);
void func_001362F8(struct S *, int);
void FireBentleyDart(struct S *p, int a) {
    void *pv;
    struct O *po;
    struct Q *pq;
    func_001356E8(p);
    switch (p->f16d8) {
    case 0: pv = p->f16dc; break;
    case 1: pv = p->f16e0; break;
    default: return;
    }
    if (!pv) return;
    po = PloAcquireProxy(pv, 0);
    if (!po) return;
    po->pe->f18 = a;
    pq = p->pq;
    pq->f60 = 0;
    pq->v50 = p->v16f0;
    LaunchSoFromLauncher(pq, po);
    ((void (*)(void *, ti128))po->vt[36].pfn)((char *)po + po->vt[36].d, D_002A2DD0);
    PrimeBomb__FP4BOMBf(po, 4.0f);
    func_001362F8(p, 1);
    p->f1700 = D_002DDED0;
}
