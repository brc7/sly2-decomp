struct VT;
struct S {
    char pad0[0xC]; struct VT *vt;
    char pad1[0x180 - 0x10]; float f180;
    char pad2[0x1A4 - 0x184]; int k1A4; float f1A8;
    char pad3[0x208 - 0x1AC]; int n208; float t20C;
};
struct VE { short d; short pad; float *(*pf)(void *); };
struct VT { struct VE e[32]; };
extern struct { float t; float dt; } D_002DDED0;
extern float D_002C3C80[3];
extern float D_002BBB60[2];
void func_001F36E0(struct S *p);
void func_00203E08(struct S *p);
float func_002458F0(float y, float x);
float RadNormalize__Ff(float a);

void func_00203FC0(struct S *p)
{
    func_001F36E0(p);
    func_00203E08(p);
    switch (p->k1A4) {
    case 0: {
        float *a = p->vt->e[31].pf((char *)p + p->vt->e[31].d);
        float *b = p->vt->e[31].pf((char *)p + p->vt->e[31].d);
        float d = RadNormalize__Ff(p->f1A8 - func_002458F0(a[1], b[0]));
        float f;
        d = __builtin_fabsf(d);
        f = D_002C3C80[0] + d * (D_002C3C80[1] + d * D_002C3C80[2]);
        {
            float lo = D_002BBB60[0], hi = D_002BBB60[1];
            if (f < lo)
                f = lo;
            else if (hi < f)
                f = hi;
        }
        p->f180 = f;
        break;
    }
    case -1:
    case 1:
        p->f180 = 1.0f;
        break;
    }
    if (p->n208)
        p->t20C = D_002DDED0.t;
}
