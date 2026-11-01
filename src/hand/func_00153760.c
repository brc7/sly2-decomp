typedef struct LM { float gMin; float gMax; } LM;
struct S { char pad[0x234]; LM lm; int i23C; float ag[16]; int c280; };
float GLimitLm__FP2LMf(LM *plm, float g);
static inline int Max(int a, int b) { return a < b ? b : a; }
/** @sly2-annot addr=0x153760
 * Purpose: Average of the last n (+0x280) scores clamped to lm
 * Info: proposed name GAverageRhythmScore (T1); TU rhythm
 * Confidence: high [T1 0.85] - evidence: semantic review; sources: r2_131F70+verdicts_tu_r1r2
 * See: docs/engine/gameplay-objects.md#key-functions,
 *   docs/reference/functions/gameplay-objects.md#rhythm-0x1520c8-0x1539a8
 * @end-sly2-annot */
float func_00153760(struct S *p)
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
