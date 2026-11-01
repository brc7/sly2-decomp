struct VTE { short d; short p; void (*pfn)(void *); };
struct VT { VTE a[3]; };
struct P { char pad[0xC]; VT *pvt; char pad2[0x30 - 0x10]; int n30; };
extern "C" void func_001F0FA0(P *);
extern "C" void func_001F0D88(P *, int);
struct Lock {
    P *p;
    Lock(P *pp) { p = pp; p->pvt->a[1].pfn((char *)p + p->pvt->a[1].d); }
    ~Lock() { p->pvt->a[2].pfn((char *)p + p->pvt->a[2].d); }
};
/** @sly2-annot addr=0x1F1078
 * Purpose: On msg 2 (ASEGA finished) for the current ASEGA: EndSmaTransition, ChooseSmaTransition
 * Info: proposed name HandleSmaMessage (T1); class SMA slot5; TU sm
 * Confidence: high [T1 0.85] - evidence: semantic review; sources: semantic_tu_docs/sm
 * See: docs/reference/functions/animation.md#sm-0x1efa90-0x1f1768
 * @end-sly2-annot */
extern "C" void func_001F1078(P *p, int k, int n)
{
    if (k == 2 && n == p->n30) {
        Lock l(p);
        func_001F0FA0(p);
        func_001F0D88(p, 0);
    }
}
