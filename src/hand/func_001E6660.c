struct S { char pad[0xE4]; int f; int c; float *ag; };
float GModPositive__Fff(float g, float gMax);
float func_001E6660(struct S *p, float g)
{
    float gMin, gMax;
    if (!p->f) return g;
    gMin = p->ag[0];
    gMax = p->ag[p->c - 1];
    if (gMin <= g && g <= gMax) return g;
    return gMin + GModPositive__Fff(g - gMin, gMax - gMin);
}
