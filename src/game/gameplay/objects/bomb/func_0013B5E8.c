// (Claude agent) CsbiIntersectSphereBsp takes its float before the HIT* (see AGENT_GUIDE "Float vs. int parameter order").
// Radius: fold turns `b >= a ? b : a` into MAX(b, a) only with both values in locals (a loaded first).
typedef int qword __attribute__((mode(TI)));
struct AREQ { int cb; int cbAlign; };
struct X { char pad[0x60]; int x60; };
struct O { char pad[0xEC]; struct X *px; };
struct S { int x0; struct O *po; int x8; int xC; char pad[0x74 - 0x10]; int x74; };
struct P { char pad[0xEC]; struct X *px; char pad1[0x15C - 0xF0]; float g15C; float g160; char pad2[0x180 - 0x164]; qword q180; };
struct HIT { int a[12]; };
extern char D_003DD490[];
void func_0020FA10();
void IntersectSwBoundingSphere(void *pg, qword q, void (*pfn)(), struct X *px, float g, int *pc, struct S ***paps);
void *func_001AE318(struct AREQ *preq);
int FIsBasicDerivedFrom__FP5BASIC3CID(int n, int k);
int CsbiIntersectSphereBsp(qword q, struct S *ps, int n, int a, int b, int c, float g, struct HIT *phit);
void func_0013B7B8(struct P *p, int *pn, struct O **apo);
/** @sly2-annot addr=0x13B5E8
 * Purpose: Objects within max(radius, radiusJt) of the detonation point (IntersectSwBoundingSphere), filtered
 *   by line of sight, sorted/uniqued
 * Info: proposed name CollectBombVictims (T2); TU bomb
 * Notes: names_v2 C proposed LoadAloAloxFromBrx__FP3ALOP18CBinaryInputStream (0.168)
 * Confidence: low (draft) [T2 0.50] - evidence: static naming pass; sources: r2b_rest
 * See: docs/reference/functions/gameplay-objects.md#bomb-0x13a9e8-0x13bdf0
 * @end-sly2-annot */
void func_0013B5E8(struct P *p, int *pc, struct O ***papo)
{
    struct AREQ req;
    int c;
    struct S **aps;
    int n;
    struct O **apo;
    int i;
    { float a = p->g15C; float b = p->g160; IntersectSwBoundingSphere(D_003DD490, p->q180, func_0020FA10, p->px, b >= a ? b : a, &c, &aps); }
    if (c == 0) {
        *pc = 0;
        *papo = 0;
        return;
    }
    req.cb = c * 4;
    req.cbAlign = 0x10;
    apo = (struct O **)func_001AE318(&req);
    n = 0;
    for (i = 0; i < c; i++) {
        struct S *ps = aps[i];
        struct HIT hit;
        int x60 = ps->po->px->x60;
        if (ps->xC == 1)
            continue;
        if (x60 == p->px->x60)
            continue;
        if (ps->x74 && !CsbiIntersectSphereBsp(p->q180, ps, 0x7FFFFFFF, 0, 0, 1, FIsBasicDerivedFrom__FP5BASIC3CID(x60, 0x34) ? p->g160 : p->g15C, &hit))
            continue;
        apo[n] = ps->po;
        n++;
    }
    func_0013B7B8(p, &n, apo);
    *pc = n;
    *papo = apo;
}
