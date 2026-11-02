/* RadPanTiltRFromVector-style: pan/tilt angles and length of v (Claude agent, 2026-10-17) */
typedef int qword __attribute__((mode(TI)));
static __inline__ float GetX(qword q) { float x; __asm__("" : "=r"(x) : "0"(q)); return x; }
static __inline__ float GetY(qword q) { float y; __asm__("prot3w %0,%0" : "=r"(y) : "0"(q)); return y; }
static __inline__ float GetZ(qword q) { float z; __asm__("pextuw %0,$0,%0" : "=r"(z) : "0"(q)); return z; }
float atan2f(float y, float x);
float RadNormalize__Ff(float g);
/** @sly2-annot addr=0x11EFB0
 * Purpose: Inverse of SetVectorSphere: pan, tilt, length of v
 * Info: proposed name GetVectorSphere (T1); TU vec
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8,
 *   docs/reference/functions/core-library.md#vec-0x11e198-0x11f440
 * @end-sly2-annot */
void GetVectorSphere(qword v, float *pradPan, float *pradTilt, float *pr)
{
    if (pradPan) {
        *pradPan = RadNormalize__Ff(atan2f(GetY(v), GetX(v)));
    }
    if (pradTilt) {
        qword d, l;
        float g;
        __asm__("vmul.xy %0, %1, %1\n\tvaddy.x %0, %0, %0y" : "=j"(d) : "j"(v), "j"(v)); /* 2nd "j"(v) unused: allocation only (keeps v in memory) */
        __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(l) : "j"(d));
        g = GetZ(v);
        *pradTilt = RadNormalize__Ff(atan2f(g, GetX(l)));
    }
    if (pr) {
        qword d, one, l, t;
        int g;
        __asm__("vmul.xyz %1, %2, %2\n\tvaddw.x %0, $vf0, $vf0w" : "=&j"(one), "=&j"(d) : "j"(v), "j"(v)); /* 2nd "j"(v) unused: allocation only */
        __asm__("vadday.x $ACC, %1, %1y\n\tvmaddz.x %0, %2, %1z" : "=j"(t) : "j"(d), "j"(one));
        d = t;
        __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(l) : "j"(d));
        __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(l));
        *(int *)pr = g;
    }
}

