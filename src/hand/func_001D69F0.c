typedef int qword __attribute__((mode(TI)));
extern qword D_002A2DC0;
static inline float Z(qword q) { float f; __asm__("pextuw %0, $zero, %1" : "=r"(f) : "r"(q)); return f; }
static inline qword Bc(float g) {
    union { qword q; float f; } u;
    qword k;
    __asm__("qmtc2.ni %1, %0" : "=j"(k) : "r"(g));
    u.q=k;
    return k;
}
/** @sly2-annot addr=0x1D69F0
 * Purpose: Velocity that moves a->b in dt under vertical acceleration acc
 * Info: proposed name VecToReachPosInDt (T2); TU tu_1D6848
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#tu_1d6848-0x1d6848-0x1d7238
 * @end-sly2-annot */
qword func_001D69F0(qword a, qword b, qword acc, float dt)
{
    float halfdt = dt * 0.5f;
    float accz = Z(acc);
    qword delta, axis, dot, one, proj, flat, inv, vz, r;
    float z, invdt;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(delta) : "j"(b), "j"(a));
    z = Z(delta) - (dt * halfdt) * accz;
    invdt = 1.0f / dt;
    axis = D_002A2DC0;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(dot), "=&j"(one) : "j"(delta), "j"(axis));
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(proj) : "j"(axis), "j"(dot));
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(flat) : "j"(delta), "j"(proj));
    inv = Bc(invdt);
    vz = Bc(z / dt);
    __asm__("vmulax.xyzw $ACC, %1, %2x\n\tvmaddx.xyzw %0, %3, %4x" : "=j"(r) : "j"(flat), "j"(inv), "j"(axis), "j"(vz));
    return r;
}
