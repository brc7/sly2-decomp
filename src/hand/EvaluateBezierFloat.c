static inline float GDiv(float a, float b) { float r; __asm__ __volatile__("div.s %0, %1, %2" : "=f"(r) : "f"(a), "f"(b)); return r; }
void EvaluateBezierFloat(float *pg, float *pdg, float dt, float t, float dtScale, float g0, float dg0, float g1, float dg1)
{
    float dg;
    float v;
    t = t / dt;
    dg = g1 - g0;
    v = 1.0f - t;
    if (pg) {
        float uv2 = t * v * v;
        float u2 = t * t;
        float u2v = u2 * v;
        float u3c = u2 * t;
        *pg = (v * v * v + uv2 * 3.0f) * g0 + uv2 * dt * dg0 - u2v * dt * dg1 + (u3c + u2v * 3.0f) * g1;
    }
    if (pdg) {
        float v6 = v * 6.0f;
        float u3 = t * 3.0f;
        *pdg = ((t * v6) / dt * dg + (1.0f - t * (4.0f - u3)) * dg0 + t * (u3 + -2.0f) * dg1) * dtScale;
    }
}
