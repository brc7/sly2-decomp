typedef int qword __attribute__((mode(TI)));
struct VTE { short d; short p; void *pfn; };
struct VT { struct VTE a[34]; };
struct O { char pad[0x94]; int n94; short k; char pad2[0xA0 - 0x9A]; int nA0; char pad3[0xB0 - 0xA4]; union { qword q; int a[4]; } v; };
struct P { char pad[0xC]; struct VT *pvt; char pad2[0x58 - 0x10]; struct O *po; };
int func_00230310(int);
void func_002303A0(struct P *, struct O *, int);
qword func_0022FCC8(int, int, qword);
void func_00230598(struct P *, struct O *);
void func_001F4BF8(struct P *);
void func_001F5078(struct P *);
void func_001F8668(struct P *, int);
/** @sly2-annot addr=0x1F4488
 * Purpose: SO vt35: set position-XF velocity so world velocity = v (minus current), refresh, mark phys
 * Info: proposed name SetSoVelocity (T1); class SO slot35; TU so
 * Notes: names_v2 C proposed CalculateSoTrajectoryApex__FP2SOP6VECTORfT1 (0.638)
 * Confidence: mid (draft) [T1 0.60] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F4488(struct P *p, qword q)
{
    struct O *po = p->po;
    qword cur, d;
    if (!po || po->n94 != 1) return;
    if (!func_00230310(po->k)) func_002303A0(p, po, 1);
    { float *pv = (float *)&po->v; pv[2] = 0.0f; pv[1] = 0.0f; pv[0] = 0.0f; }
    cur = ((qword (*)(void *))p->pvt->a[33].pfn)((char *)p + p->pvt->a[33].d);
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(q), "j"(cur));
    po->v.q = func_0022FCC8(0, po->nA0, d);
    func_00230598(p, po);
    func_001F4BF8(p);
    func_001F5078(p);
    func_001F8668(p, 0);
}
