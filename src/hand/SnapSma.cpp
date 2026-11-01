struct VTE { short d; short p; void (*pfn)(void *); };
struct VT { VTE a[3]; };
struct P { char pad[0xC]; VT *pvt; char pad2[0x24 - 0x10]; void *p24; char pad3[0x30 - 0x28]; void *p30; int n34; int n38; int n3C; };
extern "C" int FSmaRetracted(P *);
extern "C" int func_001F0438(void *, int);
extern "C" void func_00130B18(void *);
extern "C" void NotifySmaSpliceOnEnterState(P *, int, int);
extern "C" void ChooseSmaTransition(P *, int);
struct Lock {
    P *p;
    Lock(P *pp) { p = pp; p->pvt->a[1].pfn((char *)p + p->pvt->a[1].d); }
    ~Lock() { p->pvt->a[2].pfn((char *)p + p->pvt->a[2].d); }
};
extern "C" void SnapSma(P *p, int n)
{
    if (!FSmaRetracted(p)) {
        Lock l(p);
        int m = func_001F0438(p->p24, n);
        if (p->n34 == m && (p->n3C == m || p->n3C == -1)) return;
        {
            void *pv = p->p30;
            int old;
            if (pv) {
                p->p30 = 0;
                func_00130B18(pv);
            }
            old = p->n34;
            p->n38 = -1; p->n3C = m; p->n34 = m;
            NotifySmaSpliceOnEnterState(p, old, m);
            ChooseSmaTransition(p, 0);
        }
    }
}
