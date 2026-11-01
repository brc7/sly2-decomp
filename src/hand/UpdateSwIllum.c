typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f; } U;
typedef struct { float x, y, z; } V3;
typedef union { qword q; V3 v; } UV;
extern char D_002BBB60[];
struct S { char pad[0x3D5C]; int n3D5C; char pad2[0x3E10 - 0x3D60]; V3 hsv; int pad3; float f3E20; float f3E24; V3 rgb; float f3E34; int f3E38; };
void ConvertUserRgbToUserHsv(V3 *pin, UV *pout);
void func_001ED2D8(UV *pv);
float func_0011D948(void *p, float g);
void UpdateSwIllum(struct S *p)
{
    float k = 1.0f / 255.0f;
    V3 c;
    UV out;
    U u;
    qword r;
    c.x = p->hsv.x;
    c.y = p->hsv.y * k;
    c.z = p->hsv.z * k;
    ConvertUserRgbToUserHsv(&c, &out);
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
