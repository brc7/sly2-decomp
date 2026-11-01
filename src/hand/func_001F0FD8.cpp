struct VTE { short d; short p; void *pfn; };
struct VT { VTE a[6]; };
struct Q { char pad[0xC]; VT *pvt; };
struct P { char pad[0xC]; VT *pvt; char pad2[0x28 - 0x10]; Q *pq; };
extern "C" void func_00133508(P *, int, int);
struct Lock {
    P *p;
    Lock(P *pp) { p = pp; ((void (*)(void *))p->pvt->a[1].pfn)((char *)p + p->pvt->a[1].d); }
    ~Lock() { ((void (*)(void *))p->pvt->a[2].pfn)((char *)p + p->pvt->a[2].d); }
};
/** @sly2-annot addr=0x1F0FD8
 * Purpose: Forwards to listener +0x28's vtable slot 5, then to base 0x133508
 * Info: proposed name NotifySma (T2); class SMA slot4; TU sm
 * Notes: names_v2 v1 proposed NotifySmaSpliceOnEnterState__FP3SMAii (0.005)
 * Confidence: low (draft) [T2 0.35] - evidence: semantic review; static naming pass; sources:
 *   semantic_tu_docs/sm
 * See: docs/reference/functions/animation.md#sm-0x1efa90-0x1f1768
 * @end-sly2-annot */
extern "C" void func_001F0FD8(P *p, int a, int b)
{
    Lock l(p);
    Q *q = p->pq;
    if (q) ((void (*)(void *, int, int))q->pvt->a[5].pfn)((char *)q + q->pvt->a[5].d, a, b);
    func_00133508(p, a, b);
}
