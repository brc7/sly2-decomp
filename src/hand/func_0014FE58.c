typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f; } U;
struct X { void *po; };
extern qword D_002A2DC0;
qword func_001A62E8(void *p);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *p, int k);
qword func_0014FE58(struct X *p)
{
    U s;
    U pos;
    pos.q = func_001A62E8(p->po);
    if (FIsBasicDerivedFrom__FP5BASIC3CID(p->po, 0x34)) {
        register qword rs __asm__("vf2");
        register qword rp __asm__("vf3");
        register qword rd __asm__("vf1");
        rp = pos.q;
        rd = D_002A2DC0;
        __asm__("qmtc2.ni %1, %0" : "=j"(rs) : "r"(150.0f));
        s.q = rs;
        __asm__("vadda.xyzw $ACC, %0, $vf0\n\tvmaddx.xyzw %0, %2, %1x" : "+j"(rp) : "j"(rs), "j"(rd));
        pos.q = rp;
    }
    return pos.q;
}
