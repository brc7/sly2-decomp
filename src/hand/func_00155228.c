struct LM { int pad[8]; float g20; float g24; };
struct S { int pad[5]; struct LM *plm; };
float GGetDifficulty(struct S *p, int n);
int func_00155228(struct S *p, int n)
{
    float g = GGetDifficulty(p, n);
    return (int)(((1.0f - g) * p->plm->g20 + g * p->plm->g24) * (float)n);
}
