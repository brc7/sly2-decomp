typedef int qword __attribute__((mode(TI)));
static __inline__ float GetX(qword q) { float x; __asm__("mtc1 %1,%0" : "=f"(x) : "r"(q)); return x; }
static __inline__ float GetY(qword q) { float y; __asm__("prot3w %0,%1" : "=r"(y) : "r"(q)); return y; }
static __inline__ float GetZ(qword q) { float z; __asm__("pextuw %0,$0,%1" : "=r"(z) : "r"(q)); return z; }
static inline float sqrtf_(float x) { float r; __asm__ __volatile__("sqrt.s %0, %1" : "=&f"(r) : "f"(x)); return r; }
float atan2f(float y, float x);
float RadNormalize__Ff(float g);
/** @sly2-annot addr=0x11E958
 * Purpose: pan = atan2(y,x), tilt = atan2(z, |xy|), each RadNormalized, optional outputs
 * Info: proposed name CalculateVectorPanTilt (T1); TU vec
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r1_116918+verdicts_A1
 * See: docs/engine/core-library.md#7-key-functions,
 *   docs/reference/functions/core-library.md#vec-0x11e198-0x11f440
 * @end-sly2-annot */
void CalculateVectorPanTilt(qword v, float *pradPan, float *pradTilt)
{
    if (pradPan)
        *pradPan = RadNormalize__Ff(atan2f(GetY(v), GetX(v)));
    if (pradTilt)
        *pradTilt = RadNormalize__Ff(atan2f(GetZ(v), sqrtf_(GetX(v) * GetX(v) + GetY(v) * GetY(v))));
}
