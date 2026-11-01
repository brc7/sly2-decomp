typedef struct KEY { float t; float g; float dgIn; float dgOut; } KEY;
typedef struct T { int pad; int c; KEY *a; } T;
void EvaluateBezierFloat(float *pg, float *pdg, float dt, float t, float dtScale, float g0, float dg0, float g1, float dg1);
void func_0012CDA0(T *t, float *pg, float *pdg, float g, float dtScale)
{
    int i;
    KEY *k0, *k1;
    for (i = 1; i < t->c; i++) {
        if (g < t->a[i].t)
            break;
    }
    k1 = (KEY *)(i * 16 + (int)t->a);
    k0 = k1 - 1;
    EvaluateBezierFloat(pg, pdg, k1->t - k0->t, g - k0->t, dtScale, k0->g, k0->dgOut, k1->g, k1->dgIn);
}
