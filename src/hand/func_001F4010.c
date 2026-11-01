typedef int qword __attribute__((mode(TI)));
struct M4 { qword a[4]; };
struct T { struct M4 m; char pad[0xA4 - 0x40]; int nA4; };
struct VTE { short d; short p; int (*pfn)(void *); };
struct VT { struct VTE a[19]; };
struct P { char pad[0xC]; struct VT *pvt; char pad2[0x54 - 0x10]; struct T *pt; };
struct G { char pad[0x3D58]; int n; };
extern struct G D_003DD490;
struct M4 func_0011C168(void *, struct T *);
void func_00186820(struct P *);
/** @sly2-annot addr=0x1F4010
 * Purpose: SO vt37: root XF matrix = proxy matrix * own, bump g_sw stamp, vt18, 0x186820
 * Info: proposed name ApplySoProxy (T2); class SO slot37; TU so
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r6_1D5150
 * See: docs/formats/meshes-scene.md#4-scene-assembly-and-placement,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F4010(struct P *p, void *pv)
{
    struct T *pt = p->pt;
    pt->m = func_0011C168(pv, pt);
    { int n = ++D_003DD490.n; p->pt->nA4 = n; }
    p->pvt->a[18].pfn((char *)p + p->pvt->a[18].d);
    func_00186820(p);
}
