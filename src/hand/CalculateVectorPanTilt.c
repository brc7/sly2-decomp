typedef int qword __attribute__((mode(TI)));
static __inline__ float GetX(qword q) { float x; __asm__("mtc1 %1,%0" : "=f"(x) : "r"(q)); return x; }
static __inline__ float GetY(qword q) { float y; __asm__("prot3w %0,%1" : "=r"(y) : "r"(q)); return y; }
static __inline__ float GetZ(qword q) { float z; __asm__("pextuw %0,$0,%1" : "=r"(z) : "r"(q)); return z; }
static inline float sqrtf_(float x) { float r; __asm__ __volatile__("sqrt.s %0, %1" : "=&f"(r) : "f"(x)); return r; }
float func_002458F0(float y, float x);
float RadNormalize__Ff(float g);
void CalculateVectorPanTilt(qword v, float *pradPan, float *pradTilt)
{
    if (pradPan)
        *pradPan = RadNormalize__Ff(func_002458F0(GetY(v), GetX(v)));
    if (pradTilt)
        *pradTilt = RadNormalize__Ff(func_002458F0(GetZ(v), sqrtf_(GetX(v) * GetX(v) + GetY(v) * GetY(v))));
}
