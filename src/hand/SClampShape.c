struct S { char pad[0xE4]; int fWrap; };
float SMinShape(struct S *p);
float SMaxShape(struct S *p);
float SWrapShape(struct S *p, float g);
static __inline__ float GClamp(float g, float lo, float hi) { if (g < lo) return lo; if (g > hi) return hi; return g; }
float SClampShape(struct S *p, float g)
{
    if (p->fWrap) return SWrapShape(p, g);
    return GClamp(g, SMinShape(p), SMaxShape(p));
}
