typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; } VU;
extern float func_001E8008(void *p, float g, float h);
extern void func_001E6F10(void *p, float t, VU *ppos, VU *pdir);
extern float GRandInRange__Fff(float a, float b);
extern float D_002BE8B8;
extern float D_002BE8BC;
extern qword D_002A2DC0;
extern qword D_002A2DA0;
static __inline__ qword VLen(qword c)
{
    qword l, one;
    __asm__("vmul.xyz %0, %1, %1" : "=j"(l) : "j"(c));
    __asm__("vaddw.x %0, $vf0, $vf0w\n\tvadday.x ACC, %1, %1y\n\tvmaddz.x %1, %0, %1z" : "=&j"(one), "+j"(l));
    __asm__("vsqrt Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "=j"(l) : "j"(l));
    return l;
}
void func_00183218(void *p, float g, qword *pv, qword *pdv)
{
    VU pos, dir, u;
    qword n;
    qword k, c, l, one, r, s, a;
    float gl;
    func_001E6F10(p, func_001E8008(p, g, D_002BE8B8), &pos, &dir);
    __asm__("vopmula.xyz ACC, %1, %2\n\tvopmsub.xyz %0, %2, %1" : "=j"(c) : "j"(dir.q), "j"(D_002A2DC0));
    k = D_002A2DA0;
    l = VLen(c);
    __asm__("qmfc2.ni %0, %1" : "=r"(gl) : "j"(l));
    if (gl < 1e-4f) {
        n = k;
    } else {
        register qword rn __asm__("vf2");
        __asm__("vdiv Q, $vf0w, %2x\n\tvwaitq\n\tvmulq.xyzw %0, %1, Q" : "=j"(rn) : "j"(c), "j"(l));
        n = rn;
    }
    a = pos.q;
    {
        float t = GRandInRange__Fff(0.0f, D_002BE8BC);
        __asm__("qmtc2.ni %1, %0" : "=j"(s) : "r"(t));
        u.q = s;
        __asm__("vadda.xyzw ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(r) : "j"(a), "j"(n), "j"(s));
        *pv = r;
    }
    *pdv = D_002A2DC0;
}
