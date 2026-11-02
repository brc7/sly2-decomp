typedef int qword __attribute__((mode(TI)));
struct MAT3 { qword a[3]; };
struct MAT4 { qword a[4]; };
union U { qword q; float g; };
struct LO;
struct LOV { int pad[18]; short d9; short p9; void (*pfn9)(LO *); };
struct LO { int a[3]; LOV *pvt; };
struct X { char pad[0x60]; void *p60; };
struct S {
    char pad[0x18]; float dt; X *px;
    char pad2[0x40 - 0x20]; MAT3 mat; qword pos; qword x80; qword v; qword a;
    char pad3[0xF0 - 0xB0]; qword axis; float rad; LO *plo;
};
extern "C" {
qword func_001A62E8(LO *plo);
MAT3 *func_001A6318(LO *plo);
void LoadIdentityMatrix4(void *p, int n, MAT4 *pm, int k, float g);
MAT3 MulMatrix3Matrix3(MAT4 *pm, MAT3 *pm2);
void DecomposeRotateMatrixRad__FP7MATRIX3PfP6VECTOR(MAT3 *pm, float *pg, qword *pv);
void LaunchMatch__FP5MATCH(S *p);
}
static inline qword Bc(float g)
{
    U u;
    qword s;
    __asm__("qmtc2.ni %1, %0" : "=j"(s) : "r"(g));
    u.q = s;
    return s;
}
/** @sly2-annot addr=0x1CFD30
 * Purpose: Aim match at its target object: take target pose, decompose rotation, set launch velocity
 * Info: proposed name LaunchMatch (T1); TU rip
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources:
 *   r5_1AE130+verdicts_tu_r3r5+verdicts_A2+verify_side+names_v2-signature(verified)
 * See: docs/engine/particles-effects.md#6-key-functions,
 *   docs/reference/functions/particles-effects.md#rip-0x1cdcd0-0x1d1370
 * @end-sly2-annot */
void LaunchMatch__FP5MATCH(S *p)
{
    MAT4 m4;
    MAT3 m;
    qword d, s1, s2;
    p->pos = func_001A62E8(p->plo);
    p->mat = *func_001A6318(p->plo);
    LoadIdentityMatrix4(p->px->p60, 0, &m4, 0, p->dt);
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(m4.a[3]), "j"(p->pos));
    s1 = Bc(1.0f / p->dt);
    s2 = Bc(p->dt * -0.5f);
    __asm__("vmulax.xyzw $ACC, %1, %2x\n\tvmaddx.xyzw %0, %3, %4x" : "=j"(d) : "j"(d), "j"(s1), "j"(p->a), "j"(s2));
    p->v = d;
    m = MulMatrix3Matrix3(&m4, &p->mat);
    DecomposeRotateMatrixRad__FP7MATRIX3PfP6VECTOR(&m, &p->rad, &p->axis);
    if (0.0f < p->rad)
        p->rad += 6.28318548f;
    else
        p->rad -= 6.28318548f;
    p->plo->pvt->pfn9((LO *)((char *)p->plo + p->plo->pvt->d9));
    p->plo = 0;
}
