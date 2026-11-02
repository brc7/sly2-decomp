struct LM { float a, b; };
struct X { char pad[0xA8]; float *pg; };
struct G { char pad[0x3B0C]; struct X *px; };
extern struct G D_003DD490;
float GLimitLm__FP2LMf(struct LM *plm, float g);
struct S { int pad[5]; struct LM *plm; };
/** @sly2-annot addr=0x154DC8
 * Purpose: Stores the difficulty clamped by the DIFFICULTY LM range (GLimitLm) into the current sample
 * Info: proposed name SetDifficulty (T1); TU difficulty
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r3_1539A8
 * See: docs/reference/functions/game-progression.md#difficulty-0x154a60-0x1553f0
 * @end-sly2-annot */
void SetDifficulty(struct S *p, float g)
{
    struct G *pg = &D_003DD490;
    if (pg->px)
        *pg->px->pg = GLimitLm__FP2LMf(p->plm, g);
}
