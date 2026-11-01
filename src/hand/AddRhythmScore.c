typedef struct LM { float gMin; float gMax; } LM;
extern char D_002DE578[];
extern char D_002DEB30[];
extern float D_002BDBE0[3];
extern LM D_002BBB60;
struct S { char pad[0x234]; LM lm; int c; float ag[16]; float g280; float g284; };
float GGetDifficulty(void *p);
float GLimitLm__FP2LMf(LM *plm, float g);
float GAverageRhythmScore(struct S *p);
void BroadcastGameEvent(void *p, int n, void *q);
void *func_002332B8(void *dst, void *src, int cb);
static inline float GClampLm(LM *plm, float g)
{
    if (g < plm->gMin)
        return plm->gMin;
    if (plm->gMax < g)
        return plm->gMax;
    return g;
}
static inline float GLim(LM *plm, float g)
{
    if (g < plm->gMin)
        g = plm->gMin;
    else if (plm->gMax < g)
        g = plm->gMax;
    return g;
}
static inline float GClamp(float g, float gMin, float gMax)
{
    if (g < gMin)
        return gMin;
    if (gMax < g)
        return gMax;
    return g;
}
void AddRhythmScore(struct S *p, int a, int b, float g)
{
    float *pg;
    if (g < 0.0f) {
        float t = GGetDifficulty(D_002DE578);
        g *= GClamp(D_002BDBE0[0] + t * (D_002BDBE0[1] + t * D_002BDBE0[2]), D_002BBB60.gMin, D_002BBB60.gMax);
    }
    if (p->c == 16) {
        func_002332B8(&p->ag[0], &p->ag[1], 15 * sizeof(float));
        pg = &p->ag[p->c - 1];
    } else {
        pg = &p->ag[p->c++];
    }
    *pg = GLimitLm__FP2LMf(&p->lm, g);
    p->g284 = GAverageRhythmScore(p);
    BroadcastGameEvent(D_002DEB30, 0x36, p);
}
