typedef struct LM { float gMin; float gMax; } LM;
struct S { char pad[0x234]; LM lm; int i23C; float ag[16]; int c280; };
float GLimitLm__FP2LMf(LM *plm, float g);
static inline int Max(int a, int b) { return a < b ? b : a; }
float GAverageRhythmScore(struct S *p)
{
    float sum;
    int i, iMin;
    if (p->c280 <= 0)
        return 0.0f;
    sum = 0.0f;
    iMin = Max(p->i23C - p->c280, 0);
    for (i = p->i23C - 1; i >= iMin; i--)
        sum += p->ag[i];
    return GLimitLm__FP2LMf(&p->lm, sum / p->c280);
}
