struct GIFS;
struct CLUT { int a; int b; unsigned short cColor; short pad; int c; int cqwPal; };
void BuildImageGifs(int dbp, int dbw, int dpsm, int rrw, int rrh, int cqw, GIFS *pgifs);
/** @sly2-annot addr=0x1ECAB0
 * Purpose: existing name kept
 * Info: proposed name BuildClutGifs__FP4CLUTiP4GIFS (EXISTING); TU shd
 * Confidence: high [EXISTING 0.85] - evidence: pre-existing symbol in symbol_addrs.txt; sources:
 *   r6b_rest+verdicts_tu_r4r6
 * See: docs/engine/rendering.md#13-key-functions, docs/reference/functions/rendering.md#shd-0x1ea568-0x1ed7a8
 * @end-sly2-annot */
void BuildClutGifs(CLUT *pclut, int gsb, GIFS *pgifs)
{
    int rrw = (pclut->cColor > 16) ? 16 : 8;
    int rrh = ((pclut->cColor + (rrw - 1)) & -rrw) / rrw;
    BuildImageGifs(gsb, 1, 0, rrw, rrh, pclut->cqwPal, pgifs);
}
