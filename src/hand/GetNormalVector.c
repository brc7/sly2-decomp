// GetNormalVector MATCH (Claude agent, lane A10, 2026-10-23), from v3: y/z extracts are asm prot3w/pextuw into a plain int, then a
// union int->float so gcc emits the mtc1 itself (its latency gives the target schedule). x stays an asm mtc1 from the qword. One pin (|y| in $f0).
typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; } U;
static __inline__ float GetX(qword q) { float x; __asm__("mtc1 %1,%0" : "=f"(x) : "r"(q)); return x; }
static __inline__ float GetY(qword q) { qword t; float y; __asm__("prot3w %0,%1" : "=r"(t) : "r"(q)); __asm__("mtc1 %1,%0" : "=f"(y) : "r"(t)); return y; }
static __inline__ float GetZ(qword q) { qword t; float z; __asm__("pextuw %0,$0,%1" : "=r"(t) : "r"(q)); __asm__("mtc1 %1,%0" : "=f"(z) : "r"(t)); return z; }
static __inline__ qword Make(float x, float y, float z)
{
    qword q, t;
    __asm__("mfc1 %0,%2\n\tmfc1 %1,%3\n\tpextlw %0,%1,%0\n\tmfc1 %1,%4\n\tpcpyld %0,%1,%0"
            : "=&r"(q), "=&r"(t) : "f"(x), "f"(y), "f"(z));
    return q;
}
static inline qword Cross(qword a, qword b)
{
    qword r;
    __asm__("vopmula.xyz $ACC, %1, %2\n\tvopmsub.xyz %0, %2, %1" : "=j"(r) : "j"(a), "j"(b));
    return r;
}
static inline qword Normalize(qword v)
{
    qword d, one, l, r;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=j"(one) : "j"(v));
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(l) : "j"(d));
    __asm__("vdiv $Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, $Q" : "=j"(r) : "j"(l), "j"(v));
    return r;
}
float fabsf(float);
qword GetNormalVector(qword v)
{
    float x, y, z;
    float ax;
    qword w; U u;
    { int ci; union { int i; float f; } c;
    __asm__("prot3w %0,%1" : "=r"(ci) : "r"(v));
    c.i = ci;
    __asm__("mtc1 %1,%0" : "=f"(x) : "r"(v));
    y = c.f;
    __asm__("pextuw %0,$0,%1" : "=r"(ci) : "r"(v));
    c.i = ci;
    z = c.f; }
    ax = fabsf(x);


    {
    register float ay __asm__("$f0") = fabsf(y); /* pin */
    float az = fabsf(z);
        if (ax <= ay) {
        if (ax <= az)
            w = u.q = Make(0.0f, z, -y);
        else
            w = u.q = Make(y, -x, 0.0f);
    } else {
        if (ay <= az)
            w = u.q = Make(z, 0.0f, -x);
        else
            w = u.q = Make(y, -x, 0.0f);
    }
    }
    return Normalize(Cross(v, w));
}
