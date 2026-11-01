typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[4]; } M4;
typedef struct { float x, y, z, w; } __attribute__((aligned(16))) VEC;
struct LO;
typedef struct { short d; short pad; void *(*pfn)(struct LO *, M4 *); } VT;
struct LO { int x0, x4, x8; VT *vt; };
struct P { char pad[0x210]; struct LO *plo; char pad2[0xC]; qword pos; };
void LoadIdentityMatrix4(void *p, int a, M4 *pm, int b, float g);
static inline qword MulPt(M4 *m, qword v) { qword r; __asm__("vmulax.xyz $ACC, %1, %2x\n\tvmadday.xyz $ACC, %3, %2y\n\tvmaddaz.xyz $ACC, %4, %2z\n\tvmaddw.xyz %0, %5, $vf0w" : "=j"(r) : "j"(m->r[0]), "j"(v), "j"(m->r[1]), "j"(m->r[2]), "j"(m->r[3])); return r; }
VEC func_001B2398(struct P *p, float g)
{
    VEC r;
    if (p->plo) {
        M4 m;
        M4 m2;
        LoadIdentityMatrix4(p->plo->vt[32].pfn((struct LO *)((char *)p->plo + p->plo->vt[32].d), &m), 0, &m2, 0, g);
        *(qword *)&r = MulPt(&m2, MulPt(&m, p->pos));
    } else {
        *(qword *)&r = p->pos;
    }
    return r;
}
