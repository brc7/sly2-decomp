typedef int qword __attribute__((mode(TI)));
struct M3 { qword r[3]; };
struct O { char pad[0x40]; M3 mat; };
extern "C" M3 InvertMatrix3Cofactor(const M3 *pm);
extern "C" void *func_0023320C(void *d, const void *s, int n);
static inline M3 Mul(const M3 &a, const M3 &b) return r
{
    qword a0 = a.r[0], a1 = a.r[1], a2 = a.r[2];
    __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z"
        : "=j"(r.r[0]) : "j"(a0), "j"(a1), "j"(a2), "j"(b.r[0]));
    __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z"
        : "=j"(r.r[1]) : "j"(a0), "j"(a1), "j"(a2), "j"(b.r[1]));
    __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z"
        : "=j"(r.r[2]) : "j"(a0), "j"(a1), "j"(a2), "j"(b.r[2]));
}
extern "C" M3 *ConvertXfMat(M3 *pr, O *pa, O *pb, const M3 *pm)
{
    if (pa) {
        if (pb) {
            M3 m = Mul(Mul(InvertMatrix3Cofactor(&pb->mat), pa->mat), *pm);
            func_0023320C(pr, &m, 0x30);
        } else {
            M3 m = Mul(pa->mat, *pm);
            func_0023320C(pr, &m, 0x30);
        }
    } else if (pb) {
        M3 m = Mul(InvertMatrix3Cofactor(&pb->mat), *pm);
        func_0023320C(pr, &m, 0x30);
    } else {
        func_0023320C(pr, pm, 0x30);
    }
    return pr;
}
