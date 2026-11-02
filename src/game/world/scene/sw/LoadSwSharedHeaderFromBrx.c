extern int D_003E1C30;
extern int D_003E1C54;
extern int D_003E1C58;
int S32Read__18CBinaryInputStream(void *pbis);
/** @sly2-annot addr=0x20F5D0
 * Purpose: Read (cb, id) of a cross-level shared data block; flag reload if id changed or forced
 * Info: proposed name LoadSwSharedHeaderFromBrx (T1); TU sw
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: high [T1 0.90] - evidence: static analysis; sources: r7_1F8DD8+verdicts_tu_r7r8+parse-verified
 * See: docs/engine/world-sw.md#10-key-functions,
 *   docs/formats/level-objects.md#2-per-world-entry-header-loop-body-of-loadswfrombrx,
 *   docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void LoadSwSharedHeaderFromBrx(int a, void *pbis)
{
    int x = S32Read__18CBinaryInputStream(pbis);
    int y = S32Read__18CBinaryInputStream(pbis);
    D_003E1C30 = x;
    if (y != D_003E1C58) a = 1;
    D_003E1C54 = a;
    D_003E1C58 = y;
}
