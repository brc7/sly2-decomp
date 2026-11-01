struct ALLOCREQ { int cb; int cbAlign; };
struct A { int f0; int cMax; int c; int *a; };
void *func_001AE318(struct ALLOCREQ *);
void func_001AE5A0(void *, void *, int);
static inline int Max(int a, int b) { return a < b ? b : a; }
void Add__19CLoadCollectorArrayiPP2LO(struct A *pa, int cAdd, int *aAdd) {
    if (pa->cMax < pa->c + cAdd) {
        int cMax = pa->cMax;
        struct ALLOCREQ req;
        int *a;
        do {
            cMax = Max(cMax * 2, 32);
        } while (cMax < pa->c + cAdd);
        req.cb = cMax * 4;
        req.cbAlign = 0x10;
        a = func_001AE318(&req);
        func_001AE5A0(a, pa->a, pa->c * 4);
        pa->cMax = cMax;
        pa->a = a;
    }
    func_001AE5A0(pa->a + pa->c, aAdd, cAdd * 4);
    pa->c += cAdd;
}
