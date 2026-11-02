// Select the initial sprite frame/time, then forward that same F12 value for drawing.
struct RGBA2 { unsigned char r, g, b, a; };
struct RGBA {
    unsigned char r, g, b, a;
    RGBA(const RGBA &c) { *(RGBA2 *)this = *(const RGBA2 *)&c; }
} __attribute__((aligned(16)));
struct M23 { float a[6]; } __attribute__((aligned(16)));
struct E { char pad[8]; short h8; short hA; float fC; int x10; void *x14; };
struct F { char pad[0x18]; };
struct S { int x0; F *af; E *ae; char padc[0x20]; M23 *pm; };
static inline M23 Mul(const M23 &q, const M23 &n)
{
    float m0 = q.a[0], m1 = q.a[1], m2 = q.a[2], m3 = q.a[3], m4 = q.a[4], m5 = q.a[5];
    M23 r;
    r.a[0] = m0 * n.a[0] + m1 * n.a[2];
    r.a[1] = m0 * n.a[1] + m1 * n.a[3];
    r.a[2] = m2 * n.a[0] + m3 * n.a[2];
    r.a[3] = m2 * n.a[1] + m3 * n.a[3];
    r.a[4] = m4 * n.a[0] + m5 * n.a[2] + n.a[4];
    r.a[5] = m4 * n.a[1] + m5 * n.a[3] + n.a[5];
    return r;
}
extern "C" void func_00219FE0(float,F *pf, int z, M23 *pm, RGBA c0, RGBA c1);
/** @sly2-annot addr=0x217D98
 * Purpose: select initial frame/time then draw
 * Info: proposed name StartSpriteAnimAt (T2); TU hud
 * Notes: names_v2 C proposed DrawLineScreen__FUiUiUiUiUiUiG4RGBAi (0.168)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
extern "C" void func_00217D98(S *p, int i, int z, M23 *pm, float t, RGBA c0, RGBA c1)
{
    E *e = &p->ae[z];
    M23 m;
    e->h8 = i;
    e->x10 = 0;
    e->fC = t;
    e->hA = 0;
    M23 *pmUse;
    if (p->pm) {
        m = Mul(*pm, *p->pm);
        pmUse = &m;
    } else {
        pmUse = pm;
    }
    func_00219FE0(t, &p->af[e->h8], z, pmUse, c0, c1);
}
