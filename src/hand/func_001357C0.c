typedef int ti128 __attribute__((mode(TI)));
struct VTE { short d; short pad; void (*pfn)(); };
struct E { char pad[0x18]; int f18; };
struct O { int pad[3]; struct VTE *vt; char pad2[0xe4 - 0x10]; struct E *pe; };
struct Q { char pad[0x50]; ti128 v50; int f60; };
struct S { char pad[0x16d8]; int f16d8; void *f16dc; void *f16e0; struct Q *pq; ti128 v16f0; float f1700; };
extern ti128 D_002A2DD0;
extern float D_002DDED0;
void func_001356E8(struct S *);
struct O *func_001CB250(void *, int);
void func_001D6B00(struct Q *, struct O *);
void func_0013B5C8(struct O *, float);
void func_001362F8(struct S *, int);
/** @sly2-annot addr=0x1357C0
 * Purpose: Binocucom crossbow dart: clone the selected ACCMISS template (+0x16DC/+0x16E0), launch from the
 *   launcher CID112 toward the aim point +0x16F0 (LaunchSoFromLauncher), PrimeBomb(4.0), aim state +0x16BC =
 *   1
 * Info: proposed name FireBentleyDart (T1); TU bentley
 * Notes: v3: superseded FireBentleyProjectile | runtime5a #1/#2 CORRECTION: fires the ACCMISS dart (S53 oid
 *   0x245, tDetonate = fire + 4.0), not a BOMB; v2 FireBentleyProjectile
 * Confidence: mid (draft) [T1 0.75] - evidence: runtime (save-state RAM); sources: r2b_rest+runtime5a
 * See: docs/engine/player.md#7-bentley-bentleycpp-0x134648, docs/engine/player.md#11-key-functions,
 *   docs/reference/functions/player.md#bentley-0x134648-0x1378d8
 * @end-sly2-annot */
void func_001357C0(struct S *p, int a) {
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
    po = func_001CB250(pv, 0);
    if (!po) return;
    po->pe->f18 = a;
    pq = p->pq;
    pq->f60 = 0;
    pq->v50 = p->v16f0;
    func_001D6B00(pq, po);
    ((void (*)(void *, ti128))po->vt[36].pfn)((char *)po + po->vt[36].d, D_002A2DD0);
    func_0013B5C8(po, 4.0f);
    func_001362F8(p, 1);
    p->f1700 = D_002DDED0;
}
