struct T { int n; int *ap; };
struct S { char pad[0xE0]; struct T *pt; };
void *func_001CAEC8(struct S *p, int n);
int func_001CA930(struct S *p, void *q);
/** @sly2-annot addr=0x1CB250
 * Purpose: For a single-object PROXY: acquire a spare copy (PspareAcquire, mode 1 if f else 2) and return
 *   that copy's first object (aploSource[IcopyFromPspare]) or NULL
 * Info: proposed name PloAcquireProxy (T1); TU proxy
 * Notes: top50_round (0.60): Body: requires p->pt->n==1, then
 *   p->pt->ap[IcopyFromPspare(p,PspareAcquire(p,f?1:2))]. Callers spawn pooled objects:
 *   FireBentleyProjectile, SpawnBootyInContainer, SpawnCid119Guard, UseJtGadget, PloAcquireJt. Sibling FCan
 * Confidence: mid (draft) [T1 0.60] - evidence: semantic review; sources: r5_1AE130+top50_round
 * See: docs/engine/object-model.md#5-proxies-and-spares-object-pools,
 *   docs/reference/functions/gameplay-objects.md#turret-0x2259d8-0x226550,
 *   docs/reference/functions/guards-ai.md#tu_173158-0x173158-0x174898
 * @end-sly2-annot */
int func_001CB250(struct S *p, int f)
{
    /*B*/void *q; if (p->pt->n != 1) return 0; q = func_001CAEC8(p, f ? 1 : 2); if (!q) return 0; return p->pt->ap[func_001CA930(p, q)];/*E*/
}
