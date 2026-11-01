typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[4]; } M4;
struct O { char pad[0x40]; M4 mat; };
void InvertMatrix4Rigid(M4 *pd, M4 *ps);
static inline qword MulPos(const M4 *m, qword v)
{
    qword r;
    __asm__("vmulax.xyz $ACC, %2, %1x\n\tvmadday.xyz $ACC, %3, %1y\n\tvmaddaz.xyz $ACC, %4, %1z\n\tvmaddw.xyz %0, %5, $vf0w"
            : "=j"(r) : "j"(v), "j"(m->r[0]), "j"(m->r[1]), "j"(m->r[2]), "j"(m->r[3]));
    return r;
}
qword func_0022FC48(struct O *a, struct O *b, qword v)
{
    if (a) v = MulPos(&a->mat, v);
    if (b) {
        M4 m;
        InvertMatrix4Rigid(&m, &b->mat);
        v = MulPos(&m, v);
    }
    return v;
}
