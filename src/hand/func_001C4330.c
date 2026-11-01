static inline float sqrtf_(float x) { float r; __asm__ __volatile__("sqrt.s %0, %1" : "=&f"(r) : "f"(x)); return r; }
void CalculateSinCos__FfPfT1(float g, float *ps, float *pc);
float func_002458F0(float y, float x);
float RadNormalize__Ff(float g);
static inline float GMax(float a, float b) { return a < b ? b : a; }
void func_001C4330(void *p, float g, float *px, float *py, float *pang, float *pr, float *pr2)
{
    float s, c;
    *pr = sqrtf_(*px * *px + *py * *py);
    *pr2 = GMax(*pr2 - 0.2f, 0.0f);
    if (*pr < *pr2 || *pr == 0.0f) {
        CalculateSinCos__FfPfT1(*pang - g + 1.5707964f, &s, &c);
        *pr = *pr2;
        *px = *pr * c;
        *py = *pr * s;
    } else {
        *pang = RadNormalize__Ff(func_002458F0(*py, *px) + g - 1.5707964f);
        *pr2 = *pr;
    }
}
