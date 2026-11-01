typedef int qword __attribute__((mode(TI)));
typedef struct {
    int x0;
    int kind;
    char pad8[0x10 - 0x8];
    union { qword q; struct { void *pv; float f; } s; } u;
    char pad20[0xA0 - 0x20];
} E;
typedef struct { char pad[0x60]; void *x60; } OBJ;
qword func_0022FC48(void *, int, qword);
void EvaluateShapeS(void *, qword *, int, float);
void func_001DEF98(OBJ *p, E *e, qword *out, float *pdist)
{
    unsigned int i;
    for (i = 0; i < 2; i++, e++) {
        switch (e->kind) {
        case 0:
            out[i] = func_0022FC48(p->x60, 0, e->u.q);
            break;
        case 1:
            EvaluateShapeS(e->u.s.pv, &out[i], 0, e->u.s.f);
            break;
        }
    }
    if (pdist) {
        int f;
        __asm__("lqc2 $vf1, %1\n\tlqc2 $vf2, %2\n\tvsub.xyzw $vf2, $vf2, $vf1\n\tvmul.xyz $vf2, $vf2, $vf2\n\tvaddw.x $vf3, $vf0, $vf0w\n\tvadday.x $ACC, $vf2, $vf2y\n\tvmaddz.x $vf2, $vf3, $vf2z\n\tvsqrt $Q, $vf2x\n\tvwaitq\n\tvaddq.x $vf2, $vf0, $Q\n\tqmfc2.ni %0, $vf2" : "=r"(f) : "m"(out[1]), "m"(out[0]));
        *(int *)pdist = f;
    }
}
