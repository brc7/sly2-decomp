struct X {
    char pad[0xE0];
    int xE0;
    char pad1[0xEC - 0xE4];
    float *ag;
    char pad2[0x14C - 0xF0];
    float agSub[11];
};

int IsegFindShapeS(struct X *p, float *pa, float *pb, float time);
void CacheShapeSegment(struct X *p, int i);

static __inline__ float GLerp(float a, float b, float t)
{
    return (1.0f - t) * a + t * b;
}

static inline float ReturnValue(float value)
{
    register float input __asm__("$f4") = value;
    float result;
    __asm__("mov.s %0,%1" : "=f"(result) : "f"(input));
    return result;
}

float UFromShapeS(struct X *p, float time)
{
    float a, b;
    float g;
    int i = IsegFindShapeS(p, &a, &b, time);

    if (p->xE0 == 0) {
        {
            float fraction = a / b;
            float upper = *(volatile float *)&p->ag[i + 1];
            float lower = *(volatile float *)&p->ag[i];
            g = GLerp(lower, upper, fraction);
        }
    } else {
        int j;
        CacheShapeSegment(p, i);
        for (j = 1; j < 10; j++)
            if (a < p->agSub[j]) break;
        j--;
        {
            float part = (a - p->agSub[j]) / (p->agSub[j + 1] - p->agSub[j]);
            float index = j;
            part = (index + part) * 0.1f;
            g = GLerp(p->ag[i], p->ag[i + 1], part);
        }
    }
    return ReturnValue(g);
}
