typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[4]; } M4;
struct LO;
typedef struct { short d; short pad; M4 *(*pfn)(struct LO *); } VT;
struct LO { int x0, x4, x8; VT *vt; };
void InvertMatrix4Rigid(M4 *pm, M4 *pn);
qword func_001A6F00(struct LO *a, struct LO *b, qword v)
{
    if (a) {
        M4 *pm = a->vt[31].pfn((struct LO *)((char *)a + a->vt[31].d));
        __asm__("vmulax.xyz $ACC, %2, %1x\n\tvmadday.xyz $ACC, %3, %1y\n\tvmaddaz.xyz $ACC, %4, %1z\n\tvmaddw.xyz %0, %5, $vf0w"
                : "=j"(v) : "j"(v), "j"(pm->r[0]), "j"(pm->r[1]), "j"(pm->r[2]), "j"(pm->r[3]));
    }
    if (b) {
        M4 m;
        InvertMatrix4Rigid(&m, b->vt[31].pfn((struct LO *)((char *)b + b->vt[31].d)));
        __asm__("vmulax.xyz $ACC, %2, %1x\n\tvmadday.xyz $ACC, %3, %1y\n\tvmaddaz.xyz $ACC, %4, %1z\n\tvmaddw.xyz %0, %5, $vf0w"
                : "=j"(v) : "j"(v), "j"(m.r[0]), "j"(m.r[1]), "j"(m.r[2]), "j"(m.r[3]));
    }
    return v;
}
