typedef int qword __attribute__((mode(TI)));
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
struct W { int pad[0x28 / 4]; int n28; };
struct G { char pad[0x45CC]; struct W *p45CC; };
extern struct G D_003DD490;
struct X { qword a; qword v; };
struct Q { char pad[0x60]; struct X x60; struct X x80; char pad2[0x120 - 0xA0]; int n120; };
struct B { char pad[0xE8]; int nE8; char pad2[8]; qword *pF4; };
struct D { char pad[0x10]; void *p10; };
struct C { char pad[0xDC]; struct D *pDC; };
struct A { char pad[0xE8]; struct C *pE8; char pad2[0x294 - 0xEC]; struct B *p294; char pad3[0xFE8 - 0x298]; int nFE8; };
struct S {
    char pad[0x10]; struct A *p10; char pad2[0x1C]; int n30; char pad3[0x2C]; struct X x60; struct X x80;
    float fA0; int nA4; char pad4[0xC]; int nB4; struct Q *pB8; int nBC;
};
int func_001EF0A8(struct S *p);
void func_0017D450(struct S *p, int n);
void func_0017D8A8(struct S *p);
void func_002562A8(struct X *x);
qword func_001A62E8(void *p);
qword func_001C1EC0(struct X *s, qword v);
void func_0011EE08(qword p, qword a, qword b, qword *pc, float *pt, int n);
void func_001E6180(struct B *b);
int func_001C02D0(void *q, qword a, qword b, struct B *c);
static __inline__ float GetZ(qword q) { float z; __asm__("pextuw %0,$0,%1" : "=r"(z) : "r"(q)); return z; }
/** @sly2-annot addr=0x17DA28
 * Purpose: Plan: route, build the path curve to the next waypoint/pathlink (or a straight 2-point shape) and
 *   pick the state
 * Info: proposed name PlanBhvgoto (T1); TU guardbhv
 * Notes: runtime2/4 confirmed the BHVGOTO role on live guards (S02-S41)
 * Confidence: mid (draft) [T1 0.65] - evidence: runtime (save-state RAM); sources:
 *   r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_0017DA28(struct S *p) {
    struct X x;
    int k;
    if (p->n30 == -1) return;
    if (!(p->p10->nFE8 & 2)) return;
    if (func_001EF0A8(p)) return;
    func_0017D450(p, 0);
    func_0017D8A8(p);
    func_002562A8(&x);
    if (p->nB4 == 0) {
        x = p->x60;
        k = 1;
    } else {
        struct Q *q = p->pB8;
        x = q->x60;
        k = 2;
        if (q->n120 == 0) {
            union { qword q; float f[4]; } c;
            float t;
            qword pos = func_001A62E8(p->p10);
            qword a = func_001C1EC0(&q->x60, pos);
            qword b = func_001C1EC0(&q->x80, pos);
            int f;
            func_0011EE08(pos, a, b, &c.q, &t, 0);
            f = 0;
            if (t > 0.0f && t < 1.0f) {
                qword d, l;
                float g;
                __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(c.q), "j"(pos));
                __asm__("vmul.xy %0, %1, %1\n\tvaddy.x %0, %0, %0y" : "=j"(d) : "j"(d));
                __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(l) : "j"(d));
                __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(l));
                if (g < 50.0f) {
                    if (__builtin_fabsf(c.f[2] - GetZ(pos)) < 300.0f) f = 1;
                }
            }
            if (f) k = 3;
        }
    }
    if (k > 0) if (k < 3) {
        struct B *b = p->p10->p294;
        qword pos = func_001A62E8(p->p10);
        qword q = func_001C1EC0(&x, pos);
        if (p->nBC) {
            b->nE8 = 2;
            b->pF4[0] = pos;
            b->pF4[1] = q;
            func_001E6180(b);
        } else {
            struct C *c = p->p10->pE8;
            struct D *d = c ? c->pDC : 0;
            void *e = d ? d->p10 : 0;
            if (!func_001C02D0(e, pos, q, b)) {
                p->x80 = x;
                k = 5;
            }
        }
    }
    func_0017D450(p, k);
    p->fA0 = D_002DDED0.t;
    p->nA4 = D_003DD490.p45CC->n28;
}
