typedef struct KEY { float t; float g; float dgIn; float dgOut; } KEY;
typedef struct T { int pad; int c; KEY *a; } T;
void func_00117030(float *pg, float *pdg, float dt, float t, float dtScale, float g0, float dg0, float g1, float dg1);
/** @sly2-annot addr=0x12CDA0
 * Purpose: Evaluate via EvaluateBezierFloat
 * Info: proposed name EvaluateAcFloatKeys (T2); class AcFloatKeys; TU act
 * Notes: names_v2 C proposed InitTn__FP2TN (0.638)
 * Confidence: low (draft) [T2 0.30] - evidence: static naming pass; sources: r1_116918
 * See: docs/reference/functions/animation.md#act-0x12b3b0-0x12e600
 * @end-sly2-annot */
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
    func_00117030(pg, pdg, k1->t - k0->t, g - k0->t, dtScale, k0->g, k0->dgOut, k1->g, k1->dgIn);
}
