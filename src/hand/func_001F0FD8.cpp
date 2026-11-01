struct VTE { short d; short p; void *pfn; };
struct VT { VTE a[6]; };
struct Q { char pad[0xC]; VT *pvt; };
struct P { char pad[0xC]; VT *pvt; char pad2[0x28 - 0x10]; Q *pq; };
extern "C" void SendLoMessage(P *, int, int);
struct Lock {
    P *p;
    Lock(P *pp) { p = pp; ((void (*)(void *))p->pvt->a[1].pfn)((char *)p + p->pvt->a[1].d); }
    ~Lock() { ((void (*)(void *))p->pvt->a[2].pfn)((char *)p + p->pvt->a[2].d); }
};
extern "C" void func_001F0FD8(P *p, int a, int b)
{
    Lock l(p);
    Q *q = p->pq;
    if (q) ((void (*)(void *, int, int))q->pvt->a[5].pfn)((char *)q + q->pvt->a[5].d, a, b);
    SendLoMessage(p, a, b);
}
