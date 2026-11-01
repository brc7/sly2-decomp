typedef int qword __attribute__((mode(TI)));
typedef struct VECTOR { float x, y, z, w; } VECTOR;
typedef struct KEY { float g; float dgIn; float dgOut; } KEY;
typedef struct T { int pad; int c; float *at; KEY *ak[3]; int pad2; qword v; } T;
int func_0012C0E0(T *t, float g);
void EvaluateBezierFloat(float *pg, float *pdg, float dt, float u, float dtScale, float g0, float dg0, float g1, float dg1);
void func_0012C440(T *t, VECTOR *pv, VECTOR *pdv, float g, float dtScale)
{
    if (pv)
        *(qword *)pv = t->v;
    if (pdv) {
        pdv->z = 0;
        pdv->y = 0;
        pdv->x = 0;
    }
    if (t->c) {
        int i = func_0012C0E0(t, g);
        unsigned int j;
        for (j = 0; j < 3; j++) {
            KEY *ak = t->ak[j];
            if (ak) {
                float du = g - t->at[i - 1];
                float dt = t->at[i] - t->at[i - 1];
                KEY *k0 = &ak[i - 1];
                KEY *k1 = &ak[i];
                EvaluateBezierFloat(pv ? &(&pv->x)[j] : 0, pdv ? &(&pdv->x)[j] : 0,
                              dt, du, dtScale,
                              k0->g, k0->dgOut, k1->g, k1->dgIn);
            }
        }
    }
}
