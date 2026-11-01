/** @sly2-annot addr=0x1477B0
 * Purpose: 0 in states 2/3 else 1
 * Info: proposed name FDancerScreenInactive (T2); class DANCER; TU tu_1471C0
 * Notes: class identity from whole_game WE names/object strings: FCid68ScreenInactive ->
 *   FDancerScreenInactive
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r2b_rest+whole_game(class identity)
 * See: docs/engine/overview.md#2-the-frame-loop-exact-order,
 *   docs/reference/functions/cutscenes.md#tu_1471c0-0x1471c0-0x147e20
 * @end-sly2-annot */
int func_001477B0(char *p)
{
    switch (*(int *)(p + 0x150)) {
    case 2:
    case 3:
        return 0;
    }
    return 1;
}
