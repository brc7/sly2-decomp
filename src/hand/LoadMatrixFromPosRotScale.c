/* LoadMatrixFromPosRotScale (Claude agent, lane A29, 2026-10-24): builds a 4x4 matrix from a 3x3 rotation scaled per row by pg[0..2] plus a
   position. From v6 (aligned 2): the one register pin is on the pg[2] temp in Q7 (the target puts pg[2] in pg's dying a3).
   The dead sqc2 stores of the scales and their extra operands are commented below. */
typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[3]; } M3;
typedef struct { float m[4][4]; } M4;
static inline qword Q(int g)
{
    qword s;
    __asm__("qmtc2.ni %1, %0" : "=j"(s) : "r"(g));
    return s;
}
static inline qword Q7(int g)
{
    register int r asm("$7") = g; /* the one pin: the target reloads pg[2] into pg's dying a3 */
    qword s;
    __asm__("qmtc2.ni %1, %0" : "=j"(s) : "r"(r));
    return s;
}
static inline qword Q0(int g, qword *pu)
{
    qword s;
    __asm__("qmtc2.ni %1, %0" : "=j"(s) : "r"(g), "m"(*pu)); /* "m": ordering-only operand */
    return s;
}
static inline qword Mul(qword a, qword s)
{
    qword r;
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(r) : "j"(a), "j"(s));
    return r;
}
M4 *LoadMatrixFromPosRotScale(M4 *pm, qword pos, M3 *pmat, int *pg)
{
    union { qword q; float f[4]; } u;
    qword s0, s1, s2;
    s0 = Q0(pg[0], &u.q);
    *(qword *)pm->m[0] = Mul(pmat->r[0], s0);
    s1 = Q(pg[1]);
    __asm__("sqc2 %1, %0" : "=m"(u.q) : "j"(s0), "j"(s1)); /* "j"(s1): allocation-only operand */
    *(qword *)pm->m[1] = Mul(pmat->r[1], s1);
    s2 = Q7(pg[2]);
    *(qword *)pm->m[2] = Mul(pmat->r[2], s2);
    *(qword *)pm->m[3] = pos;
    __asm__("sqc2 %1, %0" : "=m"(u.q) : "j"(s1), "j"(s2)); /* "j"(s2): allocation-only operand */
    pm->m[0][3] = pm->m[1][3] = pm->m[2][3] = 0.0f;
    pm->m[3][3] = 1.0f;
    __asm__("sqc2 %1, %0" : "=m"(u.q) : "j"(s2), "m"(pm->m[0][3])); /* "m": ordering-only operand */
    return pm;
}
