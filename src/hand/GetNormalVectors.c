/* GetNormalVectors(normal, a, b, M3 rows *pm): builds basis rows 0/1. VF registers pinned with hard-register locals (Claude agent). */
typedef int qword __attribute__((mode(TI)));
#define CROSS(r, a, b) __asm__("vopmula.xyz $ACC, %1, %2\n\tvopmsub.xyz %0, %2, %1" : "=j"(r) : "j"(a), "j"(b))
#define DOT(d, one, v) __asm__("vmul.xyz %1, %2, %2\n\tvaddw.x %0, $vf0, $vf0w\n\tvadday.x $ACC, %1, %1y\n\tvmaddz.x %1, %0, %1z" : "=j"(one), "=j"(d) : "j"(v))
static __inline__ float FloatQ(qword q) { float g; __asm__("mtc1 %1, %0" : "=f"(g) : "r"(q)); return g; }
void GetNormalVectors(qword a, qword b, qword c, qword *pm)
{
    register qword vb __asm__("vf1") = b;
    register qword va __asm__("vf3") = a;
    register qword vc __asm__("vf4");
    register qword x __asm__("vf6");
    CROSS(x, vb, va);
    vc = c;
    {
        register qword d __asm__("vf1");
        register qword one __asm__("vf2");
        DOT(d, one, x);
        if (FloatQ(d) < 1.0e-4f)
            CROSS(x, vc, va);
    }
    {
        register qword y __asm__("vf1");
        register qword d0 __asm__("vf2");
        register qword one0 __asm__("vf4");
        register qword d1 __asm__("vf3");
        register qword one1 __asm__("vf5");
        register qword r0 __asm__("vf2");
        register qword r1 __asm__("vf3");
        CROSS(y, va, x);
        DOT(d0, one0, x);
        DOT(d1, one1, y);
        __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(d0) : "j"(d0));
        __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(d1) : "j"(d1));
        __asm__("vdiv $Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, $Q" : "=j"(r0) : "j"(d0), "j"(x));
        __asm__("vdiv $Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, $Q" : "=j"(r1) : "j"(d1), "j"(y));
        pm[0] = r0;
        pm[1] = r1;
    }
}
