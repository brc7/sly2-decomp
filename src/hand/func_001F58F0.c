typedef int qword __attribute__((mode(TI)));
struct V { float x, y, z, w; float pad[8]; };
struct O { char pad[0x40]; char sub[0x30]; qword q70; };
struct P { char pad[0x60]; struct O *po; };
struct Out { int n0; int pad[3]; qword q10; qword q20; float g30; float g34; float g38; int n3C; };
struct CLOCK { float t; };
extern struct CLOCK D_002DDED0;
qword func_0022D348(struct O *, int, qword);
void func_0011B938(void *, int, struct V *);
float func_002458F0(float, float);
/** @sly2-annot addr=0x1F58F0
 * Purpose: SO vt50: camera default info: base pos/velocity, radHome = atan2(-m01,-m00) of upright matrix,
 *   sAdjust 100, grfcpd 0, tMoveLast = g_clock.t
 * Info: proposed name GetSoCpdefi (T1); class SO slot50; TU so
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r6_1D5150
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/gameplay-objects.md#rhythm-0x1520c8-0x1539a8,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F58F0(struct P *p, struct Out *po)
{
    struct O *o = p->po;
    struct V v;
    void *ps;
    qword q = o->q70;
    po->q10 = q;
    ps = o->sub;
    po->q20 = func_0022D348(o, 0, q);
    func_0011B938(ps, 0, &v);
    po->g30 = func_002458F0(-v.y, -v.x); po->g34 = 100.0f; po->n0 = 0; po->g38 = D_002DDED0.t; po->n3C = 0;
}
