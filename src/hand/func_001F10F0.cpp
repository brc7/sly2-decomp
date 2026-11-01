struct VTE { short d; short p; void (*pfn)(void *); };
struct VT { VTE a[3]; };
struct A { char pad[0x74]; float g74; };
struct Q { char pad[0x14]; A *pa; int pad2; float g1C; };
struct P { char pad[0xC]; VT *pvt; char pad2[0x30 - 0x10]; Q *pq; };
extern "C" int func_001F0638(P *);
extern "C" void func_00130B18(Q *);
extern "C" void func_00130D30(Q *, int, float, float);
struct Lock {
    P *p;
    Lock(P *pp) { p = pp; p->pvt->a[1].pfn((char *)p + p->pvt->a[1].d); }
    ~Lock() { p->pvt->a[2].pfn((char *)p + p->pvt->a[2].d); }
};
/** @sly2-annot addr=0x1F10F0
 * Purpose: While a transition runs: if dt < remaining, seek forward and stop; else end and re-choose
 * Info: proposed name SkipSma (T1); TU sm
 * Confidence: high [T1 0.85] - evidence: semantic review; sources: semantic_tu_docs/sm
 * See: docs/reference/functions/animation.md#sm-0x1efa90-0x1f1768
 * @end-sly2-annot */
extern "C" void func_001F10F0(P *p, float t)
{
    if (!func_001F0638(p)) {
        Lock l(p);
        Q *q;
        while ((q = p->pq) != 0) {
            float d = q->pa->g74 - q->g1C;
            if (t < d) {
                func_00130D30(q, 1, t, 1.0f);
                break;
            }
            t -= d;
            func_00130B18(q);
        }
    }
}
