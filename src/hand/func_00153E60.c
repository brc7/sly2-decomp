typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; } U;
struct T { char pad[0xE0]; void *xE0; };
extern qword D_002A2DC0;
qword func_001A62E8(void *p);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *p, int k);

qword func_00153E60(struct T *p)
{
    if (!p->xE0) return func_001A62E8(p);
    if (FIsBasicDerivedFrom__FP5BASIC3CID(p->xE0, 0x38)) {
        U g;
        qword r;
        qword pos = func_001A62E8(p->xE0);
        float f = 80.0f;
        __asm__("qmtc2.ni %1, %0" : "=j"(g.q) : "r"(f));
        __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(r) : "j"(pos), "j"(D_002A2DC0), "j"(g.q));
        return r;
    }
    return func_001A62E8(p->xE0);
}
