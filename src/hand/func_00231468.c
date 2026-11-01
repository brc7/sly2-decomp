// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0 -ffast-math -mvu0-use-vf0-vf31
typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[4]; } M4;
typedef struct { char pad[0x40]; M4 m; } O;
typedef struct { char pad[0x50]; O *x50; char pad2[0x3C]; qword x90; } S;
extern qword D_002A2DD0;
qword GetXfVelocity(O *, int, qword);
static inline qword MulPos(const M4 *m, qword v)
{
    qword r;
    __asm__("vmulax.xyz $ACC, %2, %1x\n\tvmadday.xyz $ACC, %3, %1y\n\tvmaddaz.xyz $ACC, %4, %1z\n\tvmaddw.xyz %0, %5, $vf0w"
            : "=j"(r) : "j"(v), "j"(m->r[0]), "j"(m->r[1]), "j"(m->r[2]), "j"(m->r[3]));
    return r;
}
qword func_00231468(S *p)
{
    O *o = p->x50;
    if (!o) return D_002A2DD0;
    return GetXfVelocity(o, 0, MulPos(&o->m, p->x90));
}
