struct VTE { short d; short p; void *pfn; };
struct VT { struct VTE a[4]; };
struct Q { char pad[0x68]; char dl[1]; };
struct P { char pad[0xC]; struct VT *pvt; char pad2[0x24 - 0x10]; struct Q *pq; char pad3[0x30 - 0x28]; void *p30; };
struct DL;
extern struct DL D_003E126C;
int FSmaRetracted(struct P *);
void func_00130B18(void *);
void RemoveDlEntry__FP2DLPv(void *, void *);
/** @sly2-annot addr=0x1F0598
 * Purpose: If live: drop the current ASEGA, unlink from the SM list and the live list D_003E126C, send
 *   message 3, Decref
 * Info: proposed name RetractSma (T1); TU sm
 * Notes: names_v2 v1 proposed SetSmaGoal__FP3SMA3OID (0.185)
 * Confidence: high [T1 0.85] - evidence: semantic review; static naming pass; sources: semantic_tu_docs/sm
 * See: docs/engine/animation.md#7-key-functions, docs/reference/functions/animation.md#sm-0x1efa90-0x1f1768,
 *   docs/reference/functions/guards-ai.md#step-0x203948-0x205e70
 * @end-sly2-annot */
void func_001F0598(struct P *p)
{
    if (!FSmaRetracted(p)) {
        void *pv = p->p30;
        if (pv) {
            p->p30 = 0;
            func_00130B18(pv);
        }
        RemoveDlEntry__FP2DLPv(p->pq->dl, p);
        RemoveDlEntry__FP2DLPv(&D_003E126C, p);
        ((void (*)(void *, int, int, int, int, int))p->pvt->a[3].pfn)((char *)p + p->pvt->a[3].d, 3, 0, 0, 0, 0);
        ((void (*)(void *))p->pvt->a[2].pfn)((char *)p + p->pvt->a[2].d);
    }
}
