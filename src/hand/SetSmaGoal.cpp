struct VTE { short d; short p; void (*pfn)(void *); };
struct VT { VTE a[3]; };
struct F { int pad[3]; int fl; };
struct P { char pad[0xC]; VT *pvt; char pad2[0x24 - 0x10]; void *p24; char pad3[0x30 - 0x28]; void *p30; char pad4[0x3C - 0x34]; int n3C; F *pf; };
extern "C" int FSmaRetracted(P *);
extern "C" int func_001F0438(void *, int);
extern "C" void func_00130B18(void *);
extern "C" void ChooseSmaTransition(P *, int);
struct Lock {
    P *p;
    Lock(P *pp) { p = pp; p->pvt->a[1].pfn((char *)p + p->pvt->a[1].d); }
    ~Lock() { p->pvt->a[2].pfn((char *)p + p->pvt->a[2].d); }
};
/** @sly2-annot addr=0x1F0660
 * Purpose: ismsGoal = (oid==-1 ? -1 : IsmsFindSmRequired); if changed: retract the running ASEGA when the
 *   transition allows a break (flag 2), else choose a transition
 * Info: proposed name SetSmaGoal (T1); TU sm
 * Confidence: high [T1 0.85] - evidence: semantic review; sources: semantic_tu_docs/sm
 * See: docs/engine/animation.md#7-key-functions, docs/reference/functions/animation.md#sm-0x1efa90-0x1f1768
 * @end-sly2-annot */
extern "C" void SetSmaGoal(P *p, int n)
{
    if (FSmaRetracted(p)) return;
    n = n == -1 ? -1 : func_001F0438(p->p24, n);
    if (n != p->n3C) {
        Lock l(p);
        p->n3C = n;
        if (p->p30) {
            if (p->pf->fl & 2) func_00130B18(p->p30);
        } else ChooseSmaTransition(p, 0);
    }
}
