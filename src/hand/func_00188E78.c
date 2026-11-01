typedef int qword __attribute__((mode(TI)));
struct VT { struct { short d; short pad; qword (*pfn)(void *); } a[40]; };
struct PO { char pad[0xC]; struct VT *pvt; };
struct E { char pad[0xC]; struct PO *po; qword v10; qword v20; qword v30; };
extern void AdjustStepXpVelocity(void *p, struct E *ae, int i);
extern qword func_001A62E8(void *p);
static __inline__ qword VSub(qword a, qword b) { qword r; __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b)); return r; }
static __inline__ qword VCross(qword a, qword b)
{
    qword r;
    __asm__("vopmula.xyz ACC, %1, %2\n\tvopmsub.xyz %0, %2, %1" : "=j"(r) : "j"(a), "j"(b));
    return r;
}
void func_00188E78(void *p, struct E *ae, int i)
{
    struct E *pe;
    struct PO *po;
    AdjustStepXpVelocity(p, ae, i);
    pe = &ae[i];
    po = pe->po;
    if (po == p) {
        qword v = pe->v30;
        qword w = po->pvt->a[34].pfn((char *)po + po->pvt->a[34].d);
        qword x = pe->v10;
        pe->v30 = VSub(v, VCross(w, VSub(x, func_001A62E8(po))));
    }
}
