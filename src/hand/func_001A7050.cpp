typedef int qword __attribute__((mode(TI)));
struct M3 { qword r[3]; };
extern "C" M3 *func_001A6318(void *p);
extern "C" M3 func_0011B340(const M3 *b);
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
/** @sly2-annot addr=0x1A7050
 * Purpose: Same for a 3x3 matrix
 * Info: proposed name ConvertAloMat (T1); TU lo
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518
 * @end-sly2-annot */
extern "C" M3 func_001A7050(void *a, void *b, const M3 *m)
{
    M3 result;
    if (a) result = Mul(*func_001A6318(a), *m);
    else result = *m;
    if (b) {
        result = Mul(func_0011B340(func_001A6318(b)), result);
    }
    return result;
}
