typedef int qword __attribute__((mode(TI)));
struct S { qword a[5]; };
struct VTE { short d; short pad; void (*pfn)(void *, struct S *); };
struct O { int pad[3]; struct VTE *vt; };
struct P { char pad[0x260]; struct O *po; int pad2[3]; struct S s; };
qword func_0014E698(struct P *p, struct S *ps);
/** @sly2-annot addr=0x14E968
 * Purpose: If pso == psoAdjust(+0x260) copy cached cpdefi (+0x270) else pso->vt50 GetCpdefi and posAdjust =
 *   posBase + CalcCmAdjust
 * Info: proposed name GetCmCpdefi (T1); TU cm
 * Confidence: high [T1 0.85] - evidence: semantic review; sources: r2_131F70
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
void func_0014E968(struct P *p, struct O *po, struct S *ps)
{
    if (po == p->po) {
        *ps = p->s;
    } else {
        struct VTE *e = &po->vt[50];
        qword q, r;
        e->pfn((char *)po + e->d, ps);
        q = func_0014E698(p, ps);
        __asm__("vadd.xyzw %0, %1, %2" : "=j"(r) : "j"(ps->a[1]), "j"(q));
        ps->a[4] = r;
    }
}
