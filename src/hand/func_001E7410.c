struct X { char pad[0xE8]; int c; float *ag; };
extern float D_002A813C;
void EvaluateShapeU(struct X *p, int a, int b, float g);
float SFromShapeU(struct X *p, float g);
void func_001E7410(struct X *p, float (*pfn)(float, void *), void *pv, int a, int b, float *pg, float *pgOut)
{
    int i;
    float gBest = 0.0f;
    float dMin = D_002A813C;
    for (i = 0; i < p->c; i++) {
        float g = p->ag[i];
        float d = pfn(g, pv);
        if (d < dMin) {
            dMin = d;
            gBest = g;
        }
    }
    if (a || b) EvaluateShapeU(p, a, b, gBest);
    if (pg) *pg = gBest;
    if (pgOut) *pgOut = SFromShapeU(p, gBest);
}
