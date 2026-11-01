static inline float sqrtf_(float x) { float r; __asm__("sqrt.s %0, %1" : "=&f"(r) : "f"(x)); return r; }
static inline float GClamp(float g, float lo, float hi)
{
    if (g < lo) return lo;
    if (g > hi) return hi;
    return g;
}

void CalculateSinCos__FfPfT1(float rad, float *psin, float *pcos)
{
    float g = (rad + 1.5707963705062866f) * 0.31830987334251404f;
    int n = (int)g;
    float x2, s, c;
    if (g < 0.0f)
        n--;
    rad -= (float)n * 3.1415927410125732f;
    if (n & 1)
        rad = -rad;
    x2 = rad * rad;
    s = rad;
    rad *= x2;
    s += rad * -0.16666656732559204f;
    rad *= x2;
    s += rad * 0.008333025500178337f;
    rad *= x2;
    s += rad * -0.00019807413627859205f;
    rad *= x2;
    s += rad * 2.601886990305502e-06f;
    s = GClamp(s, -1.0f, 1.0f);
    c = sqrtf_(1.0f - s * s);
    if (n & 1)
        c = -c;
    *pcos = c;
    *psin = s;
}
