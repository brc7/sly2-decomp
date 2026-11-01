typedef int qword __attribute__((mode(TI)));
struct VTE { short d; short p; void *pfn; };
struct VT { struct VTE a[34]; };
struct O { char pad[0x94]; int n94; short k; char pad2[0xA0 - 0x9A]; int nA0; char pad3[0xB0 - 0xA4]; union { qword q; int a[4]; } v; };
struct P { char pad[0xC]; struct VT *pvt; char pad2[0x58 - 0x10]; struct O *po; };
int func_00230310(int);
void func_002303A0(struct P *, struct O *, int);
qword ConvertXfVec(int, int, qword);
void SlotUnsettle(struct P *, struct O *);
void func_001F4BF8(struct P *);
void func_001F5078(struct P *);
void func_001F8668(struct P *, int);
void SetSoVelocity(struct P *p, qword q)
{
    struct O *po = p->po;
    qword cur, d;
    if (!po || po->n94 != 1) return;
    if (!func_00230310(po->k)) func_002303A0(p, po, 1);
    { float *pv = (float *)&po->v; pv[2] = 0.0f; pv[1] = 0.0f; pv[0] = 0.0f; }
    cur = ((qword (*)(void *))p->pvt->a[33].pfn)((char *)p + p->pvt->a[33].d);
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(q), "j"(cur));
    po->v.q = ConvertXfVec(0, po->nA0, d);
    SlotUnsettle(p, po);
    func_001F4BF8(p);
    func_001F5078(p);
    func_001F8668(p, 0);
}
