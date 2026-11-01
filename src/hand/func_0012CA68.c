typedef int qword __attribute__((mode(TI)));
typedef struct VECTOR { float x, y, z, w; } VECTOR;
typedef struct KEY { float g; float dg; } KEY;
typedef struct T { int pad; int c; float *at; KEY *ak[3]; int pad2; qword v; } T;
int func_0012C708(T *t, float g);
void func_00117030(float *pg, float *pdg, float dt, float u, float dtScale, float g0, float dg0, float g1, float dg1);
/** @sly2-annot addr=0x12CA68
 * Purpose: Evaluate via EvaluateBezierFloat
 * Info: proposed name EvaluateAcFloatBBezier (T2); class AcFloatB; TU act
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/animation.md#act-0x12b3b0-0x12e600
 * @end-sly2-annot */
void func_0012CA68(T *t, VECTOR *pv, VECTOR *pdv, float g, float dtScale)
{
    if (pv)
        *(qword *)pv = t->v;
    if (pdv) {
        pdv->z = 0;
        pdv->y = 0;
        pdv->x = 0;
    }
    if (t->c) {
        int i = func_0012C708(t, g);
        unsigned int j;
        for (j = 0; j < 3; j++) {
            KEY *ak = t->ak[j];
            if (ak) {
                float du = g - t->at[i - 1];
                float dt = t->at[i] - t->at[i - 1];
                KEY *k0 = &ak[i - 1];
                KEY *k1 = &ak[i];
                func_00117030(pv ? &(&pv->x)[j] : 0, pdv ? &(&pdv->x)[j] : 0,
                              dt, du, dtScale,
                              k0->g, k0->dg, k1->g, k1->dg);
            }
        }
    }
}
