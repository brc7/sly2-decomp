struct LM { int pad[8]; float g20; float g24; };
struct S { int pad[5]; struct LM *plm; };
float GGetDifficulty(struct S *p, int n);
/** @sly2-annot addr=0x155228
 * Purpose: Interpolates n between two LM scale factors (g20..g24) by the current difficulty
 * Info: proposed name NScaleByDifficulty (T2); TU difficulty
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r3_1539A8
 * See: docs/reference/functions/game-progression.md#difficulty-0x154a60-0x1553f0
 * @end-sly2-annot */
int func_00155228(struct S *p, int n)
{
    float g = GGetDifficulty(p, n);
    return (int)(((1.0f - g) * p->plm->g20 + g * p->plm->g24) * (float)n);
}
