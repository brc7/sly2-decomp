typedef int qword __attribute__((mode(TI)));
static __inline__ float GetX(qword q) { float x; __asm__("mtc1 %1,%0" : "=f"(x) : "r"(q)); return x; }
static __inline__ float GetY(qword q) { float y; __asm__("prot3w %0,%1" : "=r"(y) : "r"(q)); return y; }
static __inline__ float GetZ(qword q) { float z; __asm__("pextuw %0,$0,%1" : "=r"(z) : "r"(q)); return z; }
float func_002458F0(float y, float x);
float func_0011CF60(float g);
/** @sly2-annot addr=0x11EF08
 * Purpose: Inverse of SetVectorCylind: angle, radius, z of v
 * Info: proposed name GetVectorCylind (T1); TU vec
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/core-library.md#vec-0x11e198-0x11f440
 * @end-sly2-annot */
void func_0011EF08(qword v, float *prad, float *pr, float *pz)
{
    if (prad)
        *prad = func_0011CF60(func_002458F0(GetY(v), GetX(v)));
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
