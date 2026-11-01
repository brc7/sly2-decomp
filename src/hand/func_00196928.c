/* func_00196928: largest non-negative limit from two quadratic root sets and two sqrt cone terms
   (lane 6, 2026-10-24).  v and a are VECTORs passed by value in single GPRs. */
typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; } U;

/* quadratic solver: returns the root count, roots written to the float buffer */
int CSolveQuadratic__FfffPf(float a, float b, float c, float *roots);

static inline float Sqrt(float x) { float r; __asm__("sqrt.s %0,%1" : "=&f"(r) : "f"(x)); return r; }
static inline float Max(float a, float b) { return a < b ? b : a; }
/* v.z as a float; Zin overwrites its (copied) input, Z reads it */
static inline float Zin(qword v) { float f; __asm__("pextuw %0,$zero,%0" : "=r"(f) : "0"(v)); return f; }
static inline float Z(qword v) { float f; __asm__("pextuw %0,$zero,%1" : "=r"(f) : "r"(v)); return f; }
/* length of the xy part through VU0.  The duplicated "j"(v) input is allocation-only: it makes v prefer
   a VU register class, so it is spilled to the stack (sq at entry, lqc2 reload) instead of living in $s1. */
static inline float LenXY(qword v) {
    register qword d __asm__("vf1");
    float r;
    __asm__("vmul.xy %0,%1,%1\n\tvaddy.x %0,%0,%0y" : "=&j"(d) : "j"(v), "j"(v));
    __asm__("vsqrt Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q" : "=j"(d) : "0"(d));
    __asm__("qmfc2.ni %0,%1" : "=r"(r) : "j"(d));
    return r;
}

float func_00196928(qword v, qword a, float low, float high, float slope0, float slope1) {
    U r0, r1;
    float az = Z(a) * 0.5f, naz = -az;
    float vz, best = 0.0f, l, t;
    int n;
    vz = Zin(v);
    n = CSolveQuadratic__FfffPf(naz, -low, vz, r0.f);
    if (n > 0) best = Max(r0.f[n - 1], best);
    n = CSolveQuadratic__FfffPf(az, -high, vz, r1.f);
    if (n > 0) best = Max(r1.f[n - 1], best);
    l = LenXY(v);
    t = (slope0 * l - vz) / naz;
    if (t > 0.0f) best = Max(Sqrt(t), best);
    t = (slope1 * l - vz) / az;
    if (t > 0.0f) best = Max(Sqrt(t), best);
    return best;
}
