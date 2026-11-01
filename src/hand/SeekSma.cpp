struct VTE { short d; short p; void (*pfn)(void *); };
struct VT { VTE a[3]; };
struct P { char pad[0xC]; VT *pvt; char pad2[0x24 - 0x10]; void *p24; char pad3[0x3C - 0x28]; int n3C; };
extern "C" int FSmaRetracted(P *);
extern "C" int func_001F0438(void *, int);
extern "C" void AbortSmaTransition(P *);
extern "C" void ChooseSmaTransition(P *, int);
struct Lock {
    P *p;
    Lock(P *pp) { p = pp; p->pvt->a[1].pfn((char *)p + p->pvt->a[1].d); }
    ~Lock() { p->pvt->a[2].pfn((char *)p + p->pvt->a[2].d); }
};
extern "C" void SeekSma(P *p, int n)
{
    if (!FSmaRetracted(p)) {
        Lock l(p);
        p->n3C = func_001F0438(p->p24, n);
        AbortSmaTransition(p);
        ChooseSmaTransition(p, 1);
    }
}
