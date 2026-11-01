// func_00142AF8 (Claude agent, laneC38 2026-10-24): MATCH from fresh5 (aligned 18). Every matrix product is written with Lilly's
// MATRIX3 operator* (the NRV helper from src/hand/func_0021CB18.cpp / func_001F4558.cpp: rows read into qword locals, each
// result row stored through a VU_VECTOR4 copy), and the first func_0022FD38 call is a plain assignment (no dead-copy wrapper).
// The one remaining trick is fresh3's commented clobber of the dead a0 in the c4678 block.
typedef int qword __attribute__((mode(TI)));
struct M3 { qword r[3]; };
struct CLOCK { float t; float dt; };
struct LM { float gMin; float gMax; };
struct R { char pad[0x40]; M3 m40; char pad1[0xA0 - 0x70]; void *pA0; };
struct Q { char pad[8]; R *p8; };
struct S {
    char pad[0xE0]; S *pNext; int pad1; float gE8; char rng[8]; int nF4;
};
struct P {
    char pad[0x60]; void *p60; char pad1[0xF04 - 0x64]; int nF04; char pad2[0x10DC - 0xF08];
    S *ps10DC; int pad3; int n10E4; char pad4[0x10F4 - 0x10E8]; Q *pq; void *p10F8; int pad5;
    int n1100; int n1104; int pad6[2]; float g1110; float g1114; float g1118; int pad7;
    LM lmPan; LM lmTilt; float g1130; float g1134; char pad8[0x1148 - 0x1138]; int n1148; float g114C;
    M3 m1150;
};
struct GS { char pad[0x4678]; int c4678; char pad1[0x4690 - 0x467C]; S *ps4690; };
extern "C" {
extern CLOCK D_002DDED0;
extern int D_002DD640;
extern float D_002A4D68;
extern qword D_002A2DA0;
extern S *D_003E1474;
extern GS D_003DD490;
void func_00142880(P *p);
void func_001427C8(P *p, int n, S *ps);
void func_001428D8(P *p, S *ps, void **ppv, float *pg, float *pg2);
int func_0011D910(void *p, float g);
void func_0012CFE0(Q *pq);
void func_0012D020(Q *pq);
qword func_001A62E8(void *p);
qword func_001A6F00(int n, P *p, qword q);
void CalculateVectorPanTilt(qword v, float *pa, float *pb);
float RadLimitLm(LM *plm, float g);
M3 LoadRotateMatrixPanTilt(float a, float b);
M3 func_0011B500(qword v, float g);
M3 func_0022FD38(void *pvFrom, void *pvTo, M3 *pm);
M3 InvertMatrix3Cofactor(const M3 *pm);
qword func_0011BFF0(M3 *pm);
void func_0012D108(Q *pq, qword q);
void func_00142AF8(P *p);
}
struct VU_VECTOR4 { qword qw; };
inline M3 operator*(const M3 &matL, const M3 &matR) return mat
{
    qword qwL0 = matL.r[0];
    qword qwL1 = matL.r[1];
    qword qwL2 = matL.r[2];
    VU_VECTOR4 v0;
    VU_VECTOR4 v1;
    VU_VECTOR4 v2;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
            : "=j"(v0.qw) : "j"(qwL0), "j"(qwL1), "j"(qwL2), "j"(((VU_VECTOR4 *)&matR.r[0])->qw));
    *(VU_VECTOR4 *)&mat.r[0] = v0;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
            : "=j"(v1.qw) : "j"(qwL0), "j"(qwL1), "j"(qwL2), "j"(((VU_VECTOR4 *)&matR.r[1])->qw));
    *(VU_VECTOR4 *)&mat.r[1] = v1;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
            : "=j"(v2.qw) : "j"(qwL0), "j"(qwL1), "j"(qwL2), "j"(((VU_VECTOR4 *)&matR.r[2])->qw));
    *(VU_VECTOR4 *)&mat.r[2] = v2;
}
static inline float GFrac(float g, float a, float b)
{
    if (b == a)
        return 0.5f;
    return (g - a) / (b - a);
}
static inline float X(const M3 &m, int i) { return ((float *)&m.r[0])[i]; }

void func_00142AF8(P *p)
{
    int f;
    float pan, tilt;
    float g;
    if (p->pq == 0)
        return;
    f = 0;
    if (p->lmPan.gMin < p->lmPan.gMax || p->lmTilt.gMin < p->lmTilt.gMax)
        f = 1;
    if (p->nF04 != -1)
        f = 0;
    if (p->g114C < D_002DDED0.t && (p->n1148 != 0 || f)) {
        p->n1148 = !p->n1148;
        func_00142880(p);
    }
    if (p->n1100 == 0) {
        if (D_002DD640 != 0) {
            func_001427C8(p, -1, 0);
        } else {
            S *psBest = p->ps10DC;
            int nBest = p->n10E4;
            float gBest, g1, g2;
            S *ps;
            if (psBest)
                func_001428D8(p, psBest, &p->p10F8, &gBest, 0);
            else
                gBest = D_002A4D68;
            for (ps = D_003E1474; ps; ps = ps->pNext) {
                if (ps->nF4 < nBest)
                    continue;
                func_001428D8(p, ps, &p->p10F8, &g1, &g2);
                if (g1 > ps->gE8)
                    continue;
                if (!func_0011D910(ps->rng, g2))
                    continue;
                if (nBest < ps->nF4 || (ps->nF4 == nBest && g1 < gBest)) {
                    psBest = ps;
                    gBest = g1;
                    nBest = psBest->nF4;
                }
            }
            if (D_003DD490.c4678 > 0 && nBest < 3) {
                __asm__("" : : : "$4"); /* clobber of dead a0 (Claude agent): target keeps &D_003DD490 in a1 */
                ps = D_003DD490.ps4690;
                if (ps) {
                    float g3, g4;
                    func_001428D8(p, ps, &p->p10F8, &g3, &g4);
                    if (nBest < 2 || (nBest == 2 && g3 < gBest)) {
                        psBest = ps;
                        nBest = 2;
                        gBest = g3;
                    }
                }
            }
            func_001427C8(p, nBest, psBest);
        }
    }
    if (p->n1104 != 0) {
        func_0012CFE0(p->pq);
        pan = p->g1110;
        tilt = p->g1114;
        g = p->g1118;
    } else {
        if (p->p10F8 == 0 || p->n1148 == 0 || !f) {
            func_0012D020(p->pq);
            return;
        }
        func_0012CFE0(p->pq);
        CalculateVectorPanTilt(func_001A6F00(0, p, func_001A62E8(p->p10F8)), &pan, &tilt);
        pan = RadLimitLm(&p->lmPan, pan);
        tilt = RadLimitLm(&p->lmTilt, tilt);
        {
            float t;
            if (p->lmPan.gMax == p->lmPan.gMin)
                t = 0.5f;
            else
                t = (pan - p->lmPan.gMin) / (p->lmPan.gMax - p->lmPan.gMin);
            float u = t * __builtin_fabsf(t);
            g = (1.0f - u) * p->g1130 + u * p->g1134;
        }
    }
    M3 m0, m1;
    m0 = LoadRotateMatrixPanTilt(pan, tilt) * func_0011B500(D_002A2DA0, g);
    m1 = func_0022FD38(0, p->p60, &p->pq->p8->m40);
    m1 = m1 * InvertMatrix3Cofactor(&p->m1150);
    M3 m2;
    if (X(m0, 0) + X(m1, 0) < 0.0f && X(m0, 1) * X(m1, 1) < 0.0f)
        m2 = p->m1150;
    else
        m2 = m0 * p->m1150;
    m2 = func_0022FD38(p->p60, p->pq->p8->pA0, &m2);
    func_0012D108(p->pq, func_0011BFF0(&m2));
}
