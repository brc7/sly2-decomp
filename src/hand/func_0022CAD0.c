typedef int qword __attribute__((mode(TI)));
static __inline__ float GetX(qword q) { float x; __asm__("mtc1 %1,%0" : "=f"(x) : "r"(q)); return x; }
static __inline__ float GetY(qword q) { float y; __asm__("prot3w %0,%1" : "=r"(y) : "r"(q)); return y; }
static __inline__ float GetZ(qword q) { float z; __asm__("pextuw %0,$0,%1" : "=r"(z) : "r"(q)); return z; }
static __inline__ qword Make(float x, float y, float z)
{
    qword q, t;
    __asm__("mfc1 %0,%2\n\tmfc1 %1,%3\n\tpextlw %0,%1,%0\n\tmfc1 %1,%4\n\tpcpyld %0,%1,%0"
            : "=&r"(q), "=&r"(t) : "f"(x), "f"(y), "f"(z));
    return q;
}
void CalculateSinCos__FfPfT1(float g, float *ps, float *pc);
qword func_0022CAD0(qword a, qword b)
{
    float s0, c0, s1, c1;
    union { qword q; float f[4]; } r;
    CalculateSinCos__FfPfT1(GetY(a), &s0, &c0);
    CalculateSinCos__FfPfT1(GetZ(a), &s1, &c1);
    r.q = Make(c0 * c1 * GetX(b) - s1 * GetY(b),
               c0 * s1 * GetX(b) + c1 * GetY(b),
               -s0 * GetX(b) + GetZ(b));
    return r.q;
}
