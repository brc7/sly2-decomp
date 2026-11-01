struct Q { char pad[0x2C]; float g2c; float g30; float g34; float g38; char p1[8]; float g44; float g48; char p2[0x14]; struct Q *next; };
struct E { int a; int b; int c; int d; struct Q *pqFirst; void *pqLast; int ib; int x1c; float g; int x24; float g28; float h; };
struct S;
float SMaxShape(int n);
static __inline__ float GClamp(float g, float lo, float hi)
{
    if (g < lo) return lo;
    if (g > hi) return hi;
    return g;
}
void func_00181CB0(struct S *p, struct E *pe)
{
    float g = SMaxShape(pe->x24);
    float r0 = g * 0.5f / (float)pe->d;
    struct Q *q;
    int i;
    float r = pe->g < r0 ? pe->g : r0;
    for (i = 0, q = pe->pqFirst; q; q = q->next, i++) {
        int j = pe->d - i - 1;
        float lo, hi, x;
        float t = (pe->g28 - q->g34) / (q->g38 - q->g34);
        if (t < 0.0f) t = 0.0f;
        else if (t > 1.0f) t = 1.0f;
        lo = (2.0f * (float)i + 1.0f) * r;
        hi = g - (2.0f * (float)j + 1.0f) * r;
        x = (1.0f - t) * q->g2c + t * q->g30;
        if (x < lo) x = lo;
        else if (hi < x) x = hi;
        q->g44 = x;
        x = (q->g2c + q->g30) * 0.5f;
        q->g48 = -__builtin_fabsf(x - pe->g28);
    }
}
