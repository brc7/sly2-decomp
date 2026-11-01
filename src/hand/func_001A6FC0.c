typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[3]; } M3;
M3 *func_001A6318(void *p);
void InvertMatrix3Cofactor(M3 *pm, M3 *pn);
qword func_001A6FC0(void *a, void *b, qword v)
{
    if (a) {
        M3 *pm = func_001A6318(a);
        __asm__("vmulax.xyz $ACC, %2, %1x\n\tvmadday.xyz $ACC, %3, %1y\n\tvmaddz.xyz %0, %4, %1z"
                : "=j"(v) : "j"(v), "j"(pm->r[0]), "j"(pm->r[1]), "j"(pm->r[2]));
    }
    if (b) {
        M3 m;
        InvertMatrix3Cofactor(&m, func_001A6318(b));
        __asm__("vmulax.xyz $ACC, %2, %1x\n\tvmadday.xyz $ACC, %3, %1y\n\tvmaddz.xyz %0, %4, %1z"
                : "=j"(v) : "j"(v), "j"(m.r[0]), "j"(m.r[1]), "j"(m.r[2]));
    }
    return v;
}
