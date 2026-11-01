struct S { char pad[0xE4]; int fWrap; };
float SMaxShape(struct S *p);
float func_001E7B38(struct S *p, float a, float b)
{
    float d = b - a;
    if (p->fWrap) {
        float r = SMaxShape(p);
        if (d > r * 0.5f) d -= r;
        else if (d < r * -0.5f) d += r;
    }
    return d;
}
