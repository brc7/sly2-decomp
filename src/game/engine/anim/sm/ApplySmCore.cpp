struct VTE { short d; short p; void (*pfn)(void *); };
struct VTE5 { short d; short p; void (*pfn)(void *, int, int, int, int, int); };
struct VT { VTE a[3]; VTE5 a3; };
struct P { int n0; char pad[0xC - 4]; VT *pvt; char pad2[0x68 - 0x10]; char dl68[0xC]; };
struct S { int n0; int n4; int n8; VT *pvt; char pad[0x24 - 0x10]; P *p24; int n28; int n2C; int pad30; int n34; int n38; int n3C; int pad40; float f44; int n48; int pad4C; };
struct SW { char pad[0x3DDC]; char dl[0xC]; char pool[0x10]; };
extern SW D_003DD490;
extern VT D_002D9768;
extern "C" {
S *PvAllocSlotheapClear(void *);
void *func_002333B4(void *, int, int);
void AppendDlEntry__FP2DLPv(void *, S *);
void FramePtr_Assign(S *, int);
int FSmaRetracted(S *);
int func_001F0438(P *, int);
void NotifySmaSpliceOnEnterState(S *, int, int);
void ChooseSmaTransition(S *, int);
}
struct Lock {
    P *p;
    Lock(P *pp) { p = pp; p->pvt->a[1].pfn((char *)p + p->pvt->a[1].d); }
    ~Lock() { p->pvt->a[2].pfn((char *)p + p->pvt->a[2].d); }
};
/** @sly2-annot addr=0x1F01B8
 * Purpose: Takes an SMA from the g_sw pool (+0x3DDC), memsets 0x50, links it into SM and world lists, runs
 *   the ctor, sets the start state, notifies, chooses a transition
 * Info: proposed name ApplySmCore (T1); TU sm
 * Confidence: mid (draft) [T1 0.70] - evidence: semantic review; sources: semantic_tu_docs/sm
 * See: docs/reference/functions/animation.md#sm-0x1efa90-0x1f1768
 * @end-sly2-annot */
extern "C" void ApplySmCore(P *p, int a, int b, int c, int d, S **pps)
{
    S *ps = PvAllocSlotheapClear(D_003DD490.pool);
    *pps = ps;
    func_002333B4(ps, 0, 0x50);
    ps->pvt = &D_002D9768;
    ps->n28 = a;
    ps->n2C = c;
    ps->n48 = d;
    ps->n0 = 0;
    ps->n4 = 0;
    ps->p24 = p;
    ps->n38 = -1;
    ps->f44 = 1.0f;
    AppendDlEntry__FP2DLPv(D_003DD490.dl, ps);
    AppendDlEntry__FP2DLPv(p->dl68, ps);
    ps->pvt->a[1].pfn((char *)ps + ps->pvt->a[1].d);
    Lock l(p);
    FramePtr_Assign(ps, p->n0);
    ps->pvt->a3.pfn((char *)ps + ps->pvt->a3.d, 2, 0, 0, 0, 0);
    if (FSmaRetracted(ps)) return;
    if (b != -1) {
        int v = func_001F0438(p, b);
        ps->n34 = v;
        ps->n3C = v;
        NotifySmaSpliceOnEnterState(ps, -1, v);
    }
    ChooseSmaTransition(ps, 0);
}
