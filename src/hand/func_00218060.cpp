// Refresh a reused sprite at zero time, then at its clamped requested time.
typedef int qword __attribute__((mode(TI)));
struct RGBA2 { unsigned char r, g, b, a; };
struct RGBA {
    unsigned char r, g, b, a;
    RGBA() {}
    RGBA(const RGBA &c) { *(RGBA2 *)this = *(const RGBA2 *)&c; }
    RGBA &operator=(const RGBA &c){*(RGBA2 *)this=*(const RGBA2 *)&c;return *this;}
} __attribute__((aligned(16)));
struct QQ { qword a, b; };
struct Y { RGBA2 c0; RGBA2 c1; char pad8[8]; QQ q; };
struct E { int count;void *items;short h8;short hA;float fC;int x10;void *x14; };
struct F { char pad[8]; float f8; char padc[0xC]; };
struct S;
struct A { int x0; F *af; E *ae; char padc[0x18]; S *ps; char pad28[8]; };
struct S { char pad[0x84]; A *aa; };
extern "C" int func_00213E20(S *p, int k, QQ *pq, RGBA *pc0, RGBA *pc1, int c, void *ay);
extern "C" void func_00217F38(A *p, int i);
// F12 is independent of the integer slots. These pointers address real
// independent caller copies of the nontrivial by-value colors (A3/T0).
extern "C" void func_002183E8(float g, A *p, int i, QQ *pq, const RGBA *c0, const RGBA *c1);
static inline float GMin(float a, float b) { return b < a ? b : a; }
static inline float GMax(float a, float b) { return a < b ? b : a; }
/** @sly2-annot addr=0x218060
 * Purpose: refresh reused sprite at t=0 then at clamped time
 * Info: proposed name SetSpriteAnimTime (T2); TU hud
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
extern "C" void func_00218060(A *p, int i, Y *py, float g)
{
    QQ q;
    RGBA c0, c1;
    int h = p->ae[i].h8;
    if (h < 0)
        return;
    g = GMin(p->af[h].f8, GMax(0.0f, g));
    if (py) {
        q = py->q;
        *(RGBA2 *)&c0 = py->c0;
        *(RGBA2 *)&c1 = py->c1;
    } else if (!func_00213E20(p->ps, p - p->ps->aa, &q, &c0, &c1, 0, 0)) {
        return;
    }
    func_00217F38(p, i);
    p->ae[i].fC = 0.0f;
    p->ae[i].hA = 0;
    p->ae[i].x10 = 0;
    // The primary argument object is copied again for the second call.
    RGBA argument0(c0),argument1(c1);
    func_002183E8(0.0f, p, i, &q, &argument0, &argument1);
    p->ae[i].x10 = 0;
    argument0=c0;RGBA argument2(c1);
    func_002183E8(g, p, i, &q, &argument0, &argument2);
    p->ae[i].x10 = 1;
}
