typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; } VU;
struct M { int x0; char pad[0x2C]; qword v30; char pad2[4]; int x44; };
struct P { char pad[0x150]; int x150; };
extern qword D_002A2DC0;
int func_001F7588(struct P *p, struct M *pm);
qword func_001A62E8(void *p);
void func_0013B428(struct P *p, qword pos, qword n);
int FIsBasicDerivedFrom__FP5BASIC3CID(int n, int k);
void func_0013B5C8(struct P *p, float g);
static __inline__ qword VLen(qword c)
{
    qword l, one;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=&j"(l), "=&j"(one) : "j"(c));
    __asm__("vsqrt Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "=j"(l) : "j"(l));
    return l;
}
int func_0013B2F0(struct P *p, struct M *pm)
{
    int r;
    if (p->x150 == 1)
        return 0;
    r = func_001F7588(p, pm);
    if (r && (pm->x44 & 8)) {
        VU u;
        VU *pu;
        register qword n __asm__("vf1");
        qword k;
        qword c = pm->v30;
        qword l;
        float gl;
        __asm__("" : "=r"(k) : "0"(D_002A2DC0));
        l = VLen(c);
        __asm__("qmfc2.ni %0, %1" : "=r"(gl) : "j"(l));
        pu = &u;
        if (gl < 1e-4f) {
            n = k;
        } else {
            qword rn;
            __asm__("vdiv Q, $vf0w, %2x\n\tvwaitq\n\tvmulq.xyzw %0, %1, Q" : "=j"(rn) : "j"(c), "j"(l));
            n = rn;
        }
        pu->q = n;
        func_0013B428(p, func_001A62E8(p), u.q);
        if (FIsBasicDerivedFrom__FP5BASIC3CID(pm->x0, 0x4F))
            func_0013B5C8(p, 0.25f);
        else
            func_0013B5C8(p, 0.0f);
    }
    return r;
}
