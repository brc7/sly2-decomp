/* BuildOrthonormalMatrixZ (Claude agent, 2026-10-23): matched from build/hand_wip/BuildOrthonormalMatrixZ.v4.c. Separate l/l2 for the two
   lengths, plus allocation-only operands on the first Div and second Len. */
typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[3]; } M3;
extern qword D_002A2DA0;
qword GetNormalVector(qword v);
static inline qword Cross(qword a, qword b)
{
    qword r;
    __asm__("vopmula.xyz $ACC, %1, %2\n\tvopmsub.xyz %0, %2, %1" : "=j"(r) : "j"(a), "j"(b));
    return r;
}
static inline qword Len(qword v)
{
    qword d, one, l;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=j"(one) : "j"(v));
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(l) : "j"(d));
    return l;
}
/* Len for z: the repeated "j"(v) operand is allocation-only (no code); it raises z's local-alloc priority so z gets vf1 */
static inline qword LenZ(qword v)
{
    qword d, one, l;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=j"(one) : "j"(v), "j"(v));
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(l) : "j"(d));
    return l;
}
/* Div for x: the repeated "j"(l) operand is allocation-only (no code); it makes global-alloc assign l before y */
static inline qword Div1(qword v, qword l)
{
    qword r;
    __asm__("vdiv $Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, $Q" : "=j"(r) : "j"(l), "j"(v), "j"(l));
    return r;
}
static inline qword Div(qword v, qword l)
{
    qword r;
    __asm__("vdiv $Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, $Q" : "=j"(r) : "j"(l), "j"(v));
    return r;
}
static __inline__ float F(qword q) { float g; __asm__("mtc1 %1, %0" : "=f"(g) : "r"(q)); return g; }
M3 *BuildOrthonormalMatrixZ(M3 *pm, qword x, qword y)
{
    qword l, l2, z;
    register qword xn __asm__("vf4"); /* register pin: normalized x lives in vf4 on both paths */
    qword fb;
    l = Len(x);
    fb = D_002A2DA0;
    if (F(l) < 1.0e-4f)
        xn = fb;
    else
        xn = Div1(x, l);
    z = Cross(y, xn);
    pm->r[1] = z;
    l2 = LenZ(z);
    pm->r[0] = xn;
    { qword t[1]; t[0] = z; } /* dead stack copy of z (the target's sqc2 vf1,0(sp)) */
    pm->r[1] = Div(z, l2);
    if (F(l2) < 1.0e-4f)
        pm->r[1] = GetNormalVector(xn);
    pm->r[2] = Cross(pm->r[0], pm->r[1]);
    return pm;
}
