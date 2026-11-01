struct X { char pad[0xE0]; int xE0; char pad1[0xF0 - 0xE4]; float *ag; char pad2[0x14C - 0xF4]; float agSub[11]; };
int IsegFindShapeU(struct X *p, float *pa, float *pb);
void CacheShapeSegment(struct X *p, int i);
static __inline__ float GLerp(float a, float b, float t) { return (1.0f - t) * a + t * b; }
float SFromShapeU(struct X *p)
{
    float a, b;
    float g;
    int i = IsegFindShapeU(p, &a, &b);
    if (p->xE0 == 0) { g = GLerp(p->ag[i], p->ag[i + 1], a / b); } else { float u; int j; CacheShapeSegment(p, i); u = a * 10.0f / b; j = (int)u; g = GLerp(p->agSub[j], p->agSub[j + 1], u - j); g += p->ag[i]; }
    return g;
}
