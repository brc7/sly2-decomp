struct BMP { char pad[8]; unsigned short w; unsigned short h; unsigned char psm; unsigned char bw; char pad2[0x18-0xE]; int cqw; };
void func_00170650(int dbp, int dbw, int psm, int w, int h, int cqw, void *pgifs);
/** @sly2-annot addr=0x1ECBF0
 * Purpose: BuildImageGifs for a bitmap
 * Info: proposed name BuildBmpGifs (T1); TU shd
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/rendering.md#shd-0x1ea568-0x1ed7a8
 * @end-sly2-annot */
void BuildBmpGifs(struct BMP *pbmp, int dbp, void *pgifs)
{
    func_00170650(dbp, pbmp->bw, pbmp->psm, pbmp->w, pbmp->h, pbmp->cqw, pgifs);
}
