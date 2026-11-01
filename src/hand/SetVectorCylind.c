typedef int qword __attribute__((mode(TI)));
static __inline__ qword Make(float x, float y, float z)
{
    qword q, t;
    __asm__("mfc1 %0,%2\n\tmfc1 %1,%3\n\tpextlw %0,%1,%0\n\tmfc1 %1,%4\n\tpcpyld %0,%1,%0"
            : "=&r"(q), "=&r"(t) : "f"(x), "f"(y), "f"(z));
    return q;
}
void CalculateSinCos__FfPfT1(float g, float *ps, float *pc);
qword SetVectorCylind(float rad, float r, float z)
{
    union { qword q; float f[4]; } v; float gA, gB; CalculateSinCos__FfPfT1(rad, &gA, &gB); v.q = Make(gB * r, gA * r, z); return v.q;
}
