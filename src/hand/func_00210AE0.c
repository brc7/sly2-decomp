extern int *D_003E1C44;
int func_00210A88(void);
/** @sly2-annot addr=0x210AE0
 * Purpose: g_anWorldOfLevel[IlevelCur()] or -1
 * Info: proposed name NWorldCur (T1); TU sw
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638,
 *   docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0,
 *   docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
int func_00210AE0(void)
{
    int i = func_00210A88();
    if (i == -1)
        return -1;
    return D_003E1C44[i];
}
