// func_0012D508 (Claude agent, 2026-10-23): MATCH. From hand_wip v5; the virtual at vtable slot 5 takes a float 4th
// argument, and the caller passes p->g20 (that is why the target loads g20 into $f12 for the 0 < g20 test).
typedef int qword __attribute__((mode(TI)));
struct M3 { qword r[3]; };
struct VU_FLOAT {
    qword q;
    VU_FLOAT(float f) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(f)); }
};
static inline qword MulM3(const M3 &m, qword v)
{
    qword r;
    __asm__("vmulax.xyz $ACC, %2, %1x\n\tvmadday.xyz $ACC, %3, %1y\n\tvmaddz.xyz %0, %4, %1z"
        : "=j"(r) : "j"(v), "j"(m.r[0]), "j"(m.r[1]), "j"(m.r[2]));
    return r;
}
static inline qword VLen(qword c)
{
    qword l, one;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=&j"(l), "=&j"(one) : "j"(c));
    __asm__("vsqrt Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "=j"(l) : "j"(l));
    return l;
}
static inline qword AddScaledDiff(qword a, qword b, qword c, VU_FLOAT f)
{
    qword r, d;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(b), "j"(c));
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(r) : "j"(a), "j"(d), "j"(f.q));
    return r;
}
static inline float GetX(qword q)
{
    float f;
    __asm__("mtc1 %1, %0" : "=f"(f) : "r"(q));
    return f;
}

struct VTE { short d; short pad; void (*pfn)(void *, int, qword *, float); };
struct O { int x0; void *x4; int pad[4]; VTE *pvt; };
struct X {
    M3 m0;
    qword v30;
    M3 m40;
    qword v70;
    float g80, g84;
    float *pg88, *pg8C;
    O *po90;
    int x94;
    unsigned short u98;
    short s9A;
    int x9C;
    X *pxA0;
    int padA4[3];
    qword vB0;
    M3 mC0;
    qword vF0;
};
struct P { int x0; void *x4; X *px8; int xC; int pad10[3]; float g1C; float g20; X *px24; };

extern "C" {
extern qword D_002A2DD0, D_002A2DC0, D_002BC380;
void func_0022DD00(void *pv, X *px);
void func_00117120(qword a, qword da, qword b, qword db, float dt, float t, float dtScale, void *pv, void *pdv, int n);
M3 func_0011B830(qword a, qword b);
void func_0022FD38(M3 *pm, int n, X *px, M3 *pmSrc);
}

static inline void SetMat(X *px, const M3 &m)
{
    px->mC0 = m;
    px->s9A = 5;
}

static inline void Fin(P *p, M3 &mT, M3 m)
{
    mT = m;
    X *px = p->px8;
    M3 *pm = &px->mC0;
    func_0022FD38(&m, 0, px->pxA0, &mT);
    *pm = m;
    p->px8->s9A = 5;
}

/** @sly2-annot addr=0x12D508
 * Purpose: Path-following act: EvaluateBezierPos + BuildOrthonormalMatrixZ
 * Info: proposed name UpdateActPath (T2); class ActPath; TU act
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/animation.md#act-0x12b3b0-0x12e600
 * @end-sly2-annot */
extern "C" void func_0012D508(P *p)
{
    X *py;
    union { qword q; } vT;
    M3 mT;
    if (p->px24 == 0) {
        X *px = p->px8;
        X *pz;
        for (pz = px->pxA0; pz != 0 && pz->x94 != 1; pz = pz->pxA0)
            ;
        if (pz == 0) {
            M3 *pm = &p->px8->mC0;
            *pm = p->px8->m0;
            p->px8->s9A = 5;
            return;
        }
        p->px24 = pz;
    }
    py = p->px24;
    if (py->po90)
        func_0022DD00(py->po90->x4, py);
    qword v;
    switch ((short)(p->px24->u98 - 1)) {
    case 0: case 1: case 6: case 7: case 8: case 10: case 16: case 17: case 19: case 20:
        v = py->vB0;
        break;
    case 4: case 5: {
        qword w;
        switch (py->s9A) {
        case 3: case 4:
            w = py->mC0.r[1];
            break;
        case 1:
            func_00117120(py->mC0.r[0], py->mC0.r[1], py->mC0.r[2], py->vF0,
                py->g84 - py->g80, *py->pg88 - py->g80, *py->pg8C, 0, &vT.q, 0);
            w = vT.q;
            break;
        case 0: case 2: case 5:
        default:
            w = D_002A2DD0;
            break;
        }
        v = w;
        break;
    }
    default:
        __asm__("vsub.xyzw %0, $vf0, $vf0" : "=j"(v));
        break;
    }
    if (py->pxA0)
        v = MulM3(py->pxA0->m40, v);
    if (GetX(VLen(v)) < 100.0f) {
        Fin(p, mT, func_0011B830(p->px8->m40.r[0], D_002A2DC0));
    } else {
        qword u = D_002BC380;
        if (0.0f < p->g20 && 0.0f < p->g1C && py->po90) {
            O *po = py->po90;
            VTE *pe = &po->pvt[5];
            pe->pfn((char *)po + pe->d, 0, &vT.q, p->g20);
            if (py->pxA0)
                vT.q = MulM3(py->pxA0->m40, vT.q);
            u = AddScaledDiff(u, vT.q, v, VU_FLOAT(p->g1C / p->g20));
        }
        Fin(p, mT, func_0011B830(v, u));
    }
}
