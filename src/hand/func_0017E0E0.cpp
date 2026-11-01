typedef int qword __attribute__((mode(TI)));
struct X;
extern "C" {
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
extern float D_002BE76C;
extern float D_002BE770[16];
struct X { qword a; qword v; X(); };
struct T { char pad[0x40]; struct X x40; };
struct D { char pad[0x10]; void *p10; };
struct C { char pad[0xDC]; struct D *pDC; };
struct A { char pad[0xE8]; struct C *pE8; };
struct S { char pad[0x10]; struct A *p10; int pad2; struct T *p18; char pad3[0x1C]; void *p38; float f3C; };
qword func_001A62E8(void *p);
float func_002458F0(float y, float x);
qword func_001C1EC0(struct X *s, qword v);
qword func_0011E1A0(float a, float b, float c);
int func_001C0B50(void *q, qword v, qword *out);
void func_002562A8(struct X *x);
void func_0017DD38(void *p, struct X *x);
}
inline X::X() { func_002562A8(this); }
extern "C" {
}
static __inline__ float GetX(qword q) { float x; __asm__("mtc1 %1,%0" : "=f"(x) : "r"(q)); return x; }
static __inline__ float GetY(qword q) { float y; __asm__("prot3w %0,%1" : "=r"(y) : "r"(q)); return y; }
/** @sly2-annot addr=0x17E0E0
 * Purpose: Pick the farthest free point among 16 directions around the target and send BHVGOTO there
 * Info: proposed name RegoalBhv189 (T2); TU guardbhv
 * Notes: BHV class word normalised: RegoalBhvstalk -> RegoalBhv189
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
extern "C" void func_0017E0E0(struct S *p) {
    union { qword q; } hit;
    qword a, b, d, r;
    struct T *t;
    struct C *c;
    struct D *dd;
    void *q;
    float ang, best;
    unsigned int i;
    p->f3C = D_002DDED0.t;
    t = p->p18;
    a = func_001A62E8(p->p38);
    b = func_001A62E8(p->p10);
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(b), "j"(a));
    c = p->p10->pE8;
    dd = c ? c->pDC : 0;
    q = dd ? dd->p10 : 0;
    best = 0.0f;
    ang = func_002458F0(GetY(d), GetX(d));
    r = func_001C1EC0(&t->x40, b);
    for (i = 0; i < 16; i++) {
        qword o = func_0011E1A0(ang + D_002BE770[i], D_002BE76C, 0.0f);
        qword s;
        __asm__("vadd.xyzw %0, %1, %2" : "=j"(s) : "j"(b), "j"(o));
        if (func_001C0B50(q, s, &hit.q)) {
            qword e, l;
            float g;
            __asm__("vsub.xyzw %0, %1, %2" : "=j"(e) : "j"(hit.q), "j"(a));
            __asm__("vmul.xy %0, %1, %1\n\tvaddy.x %0, %0, %0y" : "=j"(e) : "j"(e));
            __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(l) : "j"(e));
            __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(l));
            if (g > best) {
                best = g;
                r = hit.q;
            }
        }
    }
    struct X x;
    x.v = r;
    func_0017DD38(t, &x);
}
