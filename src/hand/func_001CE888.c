struct R { int a, b; void *p8; };
struct G { char pad[0x4054]; char pool[4]; };
struct Q { char pad[0x15C]; char dl[4]; };
extern char D_003DD490[];
struct Q *func_001CDCD0(void *pg, int n);
void *PvAllocSlotheapClear(void *pool);
struct R *func_001CE6D0(void *pv, void *a);
void AppendDlEntry__FP2DLPv(void *pdl, void *pv);
void func_001CE9C0(struct R *r);
struct R *func_001CE888(struct G *p, void *a, struct Q *q)
{
    void *pv;
    struct R *r;
    if (!q) q = func_001CDCD0(D_003DD490, 0);
    if (!q) return 0;
    pv = PvAllocSlotheapClear(p->pool);
    if (pv) {
    r = func_001CE6D0(pv, a);
    r->p8 = q;
    AppendDlEntry__FP2DLPv(q->dl, r);
    func_001CE9C0(r);
    return r; }
    return 0;
}
