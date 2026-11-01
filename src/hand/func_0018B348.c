typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[4]; } MATRIX4;
struct O { char pad[0x60]; void *p60; char pad1[0x19A4 - 0x64]; struct X *p19A4; char pad2[0x19C0 - 0x19A8]; void *p19C0;
    char pad3[0x19CC - 0x19C4]; void *p19CC; char pad4[0x1BC8 - 0x19D0]; void *p1BC8; char pad5[0x1BD8 - 0x1BCC]; float g1BD8;
    char pad6[0x1BE0 - 0x1BDC]; qword v1BE0; qword v1BF0; qword v1C00; qword v1C10; qword v1C20; };
struct X { char pad[0x60]; void *p60; };
extern float func_0012F990(void *p, int n);
extern void func_001300B8(void *a, float g, struct O *p, float h, void *b, void *c, MATRIX4 *pm, int n);
extern qword func_0011BFF0(MATRIX4 *pm);
extern void func_0011C318(MATRIX4 *pmOut, MATRIX4 *pm);
static __inline__ qword Mul(MATRIX4 *pm, qword v)
{
    qword o;
    __asm__("vmulax.xyz $ACC, %1, %5x\n\tvmadday.xyz $ACC, %2, %5y\n\tvmaddaz.xyz $ACC, %3, %5z\n\tvmaddw.xyz %0, %4, $vf0w"
            : "=j"(o) : "j"(pm->r[0]), "j"(pm->r[1]), "j"(pm->r[2]), "j"(pm->r[3]), "j"(v));
    return o;
}
/** @sly2-annot addr=0x18B348
 * Purpose: From clip label 0x167 sample positions of 3 child XFs into x1BE0..x1C20
 * Info: proposed name CacheJtRestPose (T2); TU jt
 * Notes: verifier: purpose partly wrong: Samples ASEG label 0x167 via PredictAsegEffect for 3 XFs into
 *   0x1BE0..0x1C00, plus a derived vector and the first point re-expressed in inverse(m2). 'Rest pose'
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4_17F090+verdicts_tu_r4r6
 * See: docs/reference/functions/player.md#jt-0x188488-0x18e540
 * @end-sly2-annot */
void func_0018B348(struct O *p)
{
    MATRIX4 m0, m1, m2, mi;
    float z;
    if (p->p1BC8) {
        z = 0.0f;
        p->g1BD8 = func_0012F990(p->p1BC8, 0x167);
        func_001300B8(p->p1BC8, p->g1BD8, p, z, p->p19A4->p60, p->p60, &m0, 0);
        p->v1BE0 = m0.r[3];
        func_001300B8(p->p1BC8, p->g1BD8, p, z, p->p19CC, p->p60, &m1, 0);
        p->v1BF0 = m1.r[3];
        func_001300B8(p->p1BC8, p->g1BD8, p, z, p->p19C0, p->p60, &m2, 0);
        p->v1C00 = m2.r[3];
        p->v1C10 = func_0011BFF0(&m2);
        func_0011C318(&mi, &m2);
        p->v1C20 = Mul(&mi, p->v1BE0);
    }
}
