typedef int qword __attribute__((mode(TI)));
static __inline__ float GetX(qword q) { float x; __asm__("mtc1 %1,%0" : "=f"(x) : "r"(q)); return x; }
static __inline__ float GetY(qword q) { float y; __asm__("prot3w %0,%1" : "=r"(y) : "r"(q)); return y; }
static __inline__ float GetZ(qword q) { float z; __asm__("pextuw %0,$0,%1" : "=r"(z) : "r"(q)); return z; }
float func_002458F0(float y, float x);
float RadNormalize__Ff(float g);
void GetVectorCylind(qword v, float *prad, float *pr, float *pz)
{
    if (prad)
        *prad = RadNormalize__Ff(func_002458F0(GetY(v), GetX(v)));
    if (pr) {
        qword d, l;
        __asm__("vmul.xy %0, %1, %1\n\tvaddy.x %0, %0, %0y" : "=j"(d) : "j"(v));
        __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(l) : "j"(d));
        {
            int g;
            __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(l));
            *(int *)pr = g;
        }
    }
    if (pz)
        *pz = GetZ(v);
}
