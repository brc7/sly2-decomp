/** @sly2-annot addr=0x1441A0
 * Purpose: Current MP (*pmpCur or mpMax if unbound)
 * Info: proposed name MpStepplayerCur (T1); TU stepplayer
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
int func_001441A0(char *p)
{
    int *q = *(int **)(p + 0x12FC);
    if (!q) return *(int *)(p + 0x1300);
    return *q;
}
