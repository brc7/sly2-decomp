struct VT { short d; short pad; void *pfn; };
struct P;
struct E { int x0; int x4; struct P *p8; char pad[0x34]; };
struct A { struct E ae[2]; char pad[0xAC - 0x80]; struct A *pnext; };
struct L { struct A *pa; struct L *pnext; };
struct O { char pad[0x64]; struct L *pl; };
struct P { char pad[0xC]; struct VT *pvt; char pad1[0xEC - 0x10]; struct O *po; char pad2[0x17C - 0xF0]; struct P *p17C; };
static __inline__ int IEnd(struct A *pa, struct P *p) { return pa->ae[0].p8 == p ? 0 : 1; }
/** @sly2-annot addr=0x132298
 * Purpose: Deferred callback: collect up to 16 distinct SOs touching the barrier, and for each set +0x17C=pso
 *   and HandleSpliceEvent(5) via vt3 (fires the Splice impact event)
 * Info: proposed name SendBarrierImpactMessages (T1); TU barrier
 * Notes: names_v2 A proposed NotifyBarrierImpact__FP7BARRIERP2XPi (0.996)
 * Confidence: mid (draft) [T1 0.60] - evidence: static naming pass; sources: r2b_rest+verdicts_A1
 * See: docs/engine/gameplay-objects.md#gameplay-object-classes,
 *   docs/engine/gameplay-objects.md#key-functions,
 *   docs/engine/world-sw.md#6-deferred-callbacks-and-the-splice-schedule
 * @end-sly2-annot */
void func_00132298(struct P *p)
{
    struct P *ap[16];
    int c = 0;
    struct L *pl;
    int i;
    for (pl = p->po->pl; pl; pl = pl->pnext) {
        struct A *pa;
        for (pa = pl->pa; pa; pa = pa->pnext) {
            struct P *pT = pa->ae[1 - IEnd(pa, p)].p8;
            for (i = 0; i < c; i++)
                if (pT == ap[i])
                    break;
            if (i >= c) {
                ap[c++] = pT;
                if (c >= sizeof(ap) / sizeof(ap[0]))
                    goto done;
            }
        }
    }
done:
    for (i = 0; i < c; i++) {
        p->p17C = ap[i];
        ((void (*)(void *, int, int, int, int, int))p->pvt[3].pfn)((char *)p + p->pvt[3].d, 5, 0, 0, 0, 0);
    }
}
