typedef int qword __attribute__((mode(TI)));
struct M4 { qword a[4]; };
struct T { struct M4 m; char pad[0xA4 - 0x40]; int nA4; };
struct VTE { short d; short p; int (*pfn)(void *); };
struct VT { struct VTE a[19]; };
struct P { char pad[0xC]; struct VT *pvt; char pad2[0x54 - 0x10]; struct T *pt; };
struct G { char pad[0x3D58]; int n; };
extern struct G D_003DD490;
struct M4 __ml__FRC7MATRIX4T0(void *, struct T *);
void func_00186820(struct P *);
void func_001F4010(struct P *p, void *pv)
{
    struct T *pt = p->pt;
    pt->m = __ml__FRC7MATRIX4T0(pv, pt);
    { int n = ++D_003DD490.n; p->pt->nA4 = n; }
    p->pvt->a[18].pfn((char *)p + p->pvt->a[18].d);
    func_00186820(p);
}
