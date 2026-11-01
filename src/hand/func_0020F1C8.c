typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f; } U;
typedef struct { float x, y, z; } V3;
typedef union { qword q; V3 v; } UV;
extern char D_002BBB60[];
struct S { char pad[0x3D5C]; int n3D5C; char pad2[0x3E10 - 0x3D60]; V3 hsv; int pad3; float f3E20; float f3E24; V3 rgb; float f3E34; int f3E38; };
void func_0011DD90(V3 *pin, UV *pout);
void func_001ED2D8(UV *pv);
float func_0011D948(void *p, float g);
/** @sly2-annot addr=0x20F1C8
 * Purpose: Recompute ambient light colour from HSV (+0x3E10) scaled by midtone (+0x3E20/255) and shadow level
 *   (+0x3E24/255); bump illum change counter
 * Info: proposed name UpdateSwIllum (T1); TU sw
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/engine/rendering.md#3-drawsw-sort-prologue-and-pass-dispatch-srchandfunc_00157fa0c-cstatic,
 *   docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void func_0020F1C8(struct S *p)
{
    float k = 1.0f / 255.0f;
    V3 c;
    UV out;
    U u;
    qword r;
    c.x = p->hsv.x;
    c.y = p->hsv.y * k;
    c.z = p->hsv.z * k;
    func_0011DD90(&c, &out);
    __asm__("qmtc2.ni %1, %0" : "=j"(u.q) : "r"(p->f3E20 * k));
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(r) : "j"(out.q), "j"(u.q));
    out.q = r;
    func_001ED2D8(&out);
    p->rgb = out.v;
    p->f3E34 = p->f3E24 * k;
    if (p->f3E38)
        p->f3E34 = func_0011D948(D_002BBB60, p->f3E34 + 0.3f);
    p->n3D5C++;
}
