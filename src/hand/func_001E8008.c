static inline float StoreEndpointAndUpperBits(float *endpoint,float upper,unsigned highHalf)
{
    register unsigned bits __asm__("$1");
    register float bound __asm__("$f12");
    __asm__(".set noat\n\tswc1 %3,%0\n\tlui %2,%4\n\tmtc1 %2,%1\n\t.set at"
            :"=m"(*endpoint),"=f"(bound),"=&r"(bits):"f"(upper),"i"(highHalf));
    return bound;
}
struct P { char pad[0xE4]; int fE4; };
extern int D_002C3650;
float func_001E6648(struct P *);
float GRandInRange__Fff(float, float);
float func_001E66C8(struct P *, float);
static inline float sqrtf_(float x) { float r; __asm__ __volatile__("sqrt.s %0, %1" : "=&f"(r) : "f"(x)); return r; }
static inline float GClamp(float x, float lo, float hi)
{
    if (x < lo) return lo;
    if (hi < x) return hi;
    return x;
}
static inline float GMin(float a, float b) { return b < a ? b : a; }
static inline float GMax(float a, float b) { return a < b ? b : a; }
float func_001E8008(struct P *p, float a, float b)
{
    float r[2];
    float t,u,upper,minimum;
    {register float upper __asm__("$f0"),lower __asm__("$f1");
    if (p->fE4) {
        lower = a - b;
        upper = a + b;
    } else {
        float m = func_001E6648(p);
        float c = GClamp(a, b, m - b);
        lower = GMax(c - b, 0.0f);
        upper = GMin(c + b, m);
    }
    minimum=StoreEndpointAndUpperBits(&r[1],upper,0xBF800000u>>16);
    r[0] = lower;
    }
    t = GRandInRange__Fff(minimum, 1.0f);
    switch (D_002C3650) {
    case 0:
        break;
    case 1:
        t = t * __builtin_fabsf(t);
        break;
    case 2:
        if (0.0f < t) upper = t * t * (3.0f - (t + t));
        else upper = t * t * (-3.0f - (t + t));
        t = upper;
        break;
    case 3:
        if (0.0f < t) upper = sqrtf_(t);
        else upper = -sqrtf_(-t);
        t = upper;
        break;
    }
    t = t * 0.5f + 0.5f;
    u = (1.0f - t) * r[0] + t * r[1];
    if (p->fE4)
        u = func_001E66C8(p, u);
    return u;
}

typedef char layout_pointer[(sizeof(void*)==4)?1:-1];
typedef char layout_int[(sizeof(int)==4)?1:-1];
typedef char layout_float[(sizeof(float)==4)?1:-1];
typedef char layout_P[(sizeof(struct P)==0xE8)?1:-1];
typedef char layout_P_closed[((unsigned)&((struct P*)0)->fE4==0xE4)?1:-1];
