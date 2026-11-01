struct VTE { short d; short pad; void (*pfn)(); };
struct O { int a[3]; struct VTE *vt; };
struct B { int pad[5]; struct O *po; };
struct A { int pad[4]; struct B *pb; };
void func_0014FCC8(struct A *a, int n, struct B *b);
/** @sly2-annot addr=0x151648
 * Purpose: pcammove(+0x10)=pv; pcammove->pevent vt1 (begin event)
 * Info: proposed name ActivateCpcamblend (T1); TU cplcy
 * Notes: runtime6 C7: CPCAMBLEND active with pv = JSG CAMMOVE (S59)
 * Confidence: high [T1 0.90] - evidence: runtime (save-state RAM); semantic review; sources:
 *   r2_131F70+verdicts_tu_r1r2+runtime
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cplcy-0x14fca8-0x1520c8
 * @end-sly2-annot */
void func_00151648(struct A *a, int n, struct B *b)
{
    struct O *o;
    struct VTE *e;
    func_0014FCC8(a, n, b);
    a->pb = b;
    o = b->po;
    e = &o->vt[1];
    e->pfn((char *)o + e->d);
}
