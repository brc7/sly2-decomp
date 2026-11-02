// MATCH (Claude agent, laneB7, 2026-10-23), from fresh3 (2). One commented register pin: vel pinned to vf2 in the VU_FLOATM operator*, and the allocation-only duplicate g operand is "jm" (memory allowed, so it needs no second reload of the spilled g). The VU_FLOATM ctor is asm("lw") from "m"(f).
typedef int qword __attribute__((mode(TI)));
struct VU_FLOAT {
    qword q;
    VU_FLOAT(float f) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(f)); }
};
struct VU_VECTOR {
    qword q;
    VU_VECTOR(const qword &v) { q = v; }
};
struct VU_FLOATM {
    qword q;
    VU_FLOATM(const float &f) { __asm__("lw %0, %1" : "=r"(q) : "m"(f)); }
};
static inline qword operator*(VU_VECTOR v, VU_FLOATM f)
{
    qword r;
    register qword vv __asm__("vf2") = v.q; /* register pin: vel must be vf2 while g's reload and the output share vf1 */
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(r) : "j"(vv), "j"(f.q), "jm"(f.q));
    return r;
}
static inline qword operator*(VU_VECTOR v, VU_FLOAT f)
{
    qword r;
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(r) : "j"(v.q), "j"(f.q));
    return r;
}
struct MATRIX3 { qword r[3]; };
static inline qword operator*(const MATRIX3 &m, VU_VECTOR v)
{
    qword r;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
        : "=j"(r) : "j"(m.r[0]), "j"(m.r[1]), "j"(m.r[2]), "j"(v.q));
    return r;
}

#define V4(a) virtual void a##0(); virtual void a##1(); virtual void a##2(); virtual void a##3();
struct BASE { int x0, x4, x8; };
struct P : BASE {
    V4(a) V4(b) V4(c) V4(d) V4(e) V4(f) V4(g) V4(h)
    virtual qword GetVel();            /* 33 */
    virtual void v34();
    virtual void SetVel(qword v);      /* 35 */
};
struct PX {
    char pad[0x190];
    qword v190;
    char pad1A0[0x10];
    MATRIX3 m1B0;
    char pad1E0[0x1030 - 0x1E0];
    int a1030;
    int a1034[(0x10DC - 0x1034) / 4];
    int x10DC;
};
struct Q {
    virtual void v1();
    virtual void v2();
    virtual void Eval(int n, qword *pv, float g0, float g1);
};
struct R { int x0; Q *pq; };
struct X { char pad[0x78]; float g78; };
struct S { P *p; int k; float t; int kCur; X *px0; X *px1; };
struct CLOCK { float t; float dt; };
extern "C" {
extern CLOCK D_002DDED0;
extern float D_002BC9B8;
void FaceStepToward(P *p, int n);
R *func_0012FF10(X *px, P *p, int a, int b);
void ReleaseSma(void *pv);
void func_001305B8(X *px, P *p, int n, void *pv, float g0, float g1);
}
#define PXOF(p) ((PX *)(p))

/** @sly2-annot addr=0x1375D0
 * Purpose: state setter scaling velocity on exit
 * Info: proposed name JtHelperSetState (T2); TU bentley
 * Notes: names_v2 C proposed CatchWmDisplayState__FP2WM (0.200)
 * Confidence: low (draft) [T2 0.25] - evidence: static naming pass; sources: r2b_rest
 * See: docs/reference/functions/player.md#bentley-0x134648-0x1378d8
 * @end-sly2-annot */
extern "C" void func_001375D0(S *s, int k)
{
    X *px;
    if (k == s->k)
        return;
    s->k = k;
    px = 0;
    s->t = D_002DDED0.t;
    if (k == -1) {
        VU_FLOATM g(D_002BC9B8);
        qword vel = s->p->GetVel();
        qword r = VU_VECTOR(vel) * g;
        s->p->SetVel(r);
        return;
    }
    if (k >= -1) if (k < 2) {
        union { qword q; } v;
        if (k == 0)
            px = s->px0;
        else
            px = s->px1;
        FaceStepToward(s->p, PXOF(s->p)->x10DC);
        func_0012FF10(px, s->p, 1, 1)->pq->Eval(0, &v.q, 0.0f, px->g78);
        s->p->SetVel(PXOF(s->p)->m1B0 * VU_VECTOR(v.q));
        PXOF(s->p)->v190 = VU_VECTOR(v.q) * VU_FLOAT(-1.0f);
        s->kCur = k;
    }
    ReleaseSma(PXOF(s->p)->a1034);
    func_001305B8(px, s->p, 1, &PXOF(s->p)->a1030, 0.0f, 1.0f);
}
