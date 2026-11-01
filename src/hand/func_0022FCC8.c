typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[3]; } M3;
struct O { char pad[0x40]; M3 mat; };
void InvertMatrix3Cofactor(M3 *pd, M3 *ps);
static inline qword Mul3(const M3 *m, qword v)
{
    qword r;
    __asm__("vmulax.xyz $ACC, %2, %1x\n\tvmadday.xyz $ACC, %3, %1y\n\tvmaddz.xyz %0, %4, %1z"
            : "=j"(r) : "j"(v), "j"(m->r[0]), "j"(m->r[1]), "j"(m->r[2]));
    return r;
}
qword func_0022FCC8(struct O *a, struct O *b, qword v)
{
    if (a) v = Mul3(&a->mat, v);
    if (b) {
        M3 m;
        InvertMatrix3Cofactor(&m, &b->mat);
        v = Mul3(&m, v);
    }
    return v;
}
