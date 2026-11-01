extern int *D_003E1C48;
int IlevelCur__Fv(void);
/** @sly2-annot addr=0x210A98
 * Purpose: g_anEpisodeOfLevel[IlevelCur()] or -1
 * Info: proposed name NEpisodeCur (T1); TU sw
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0,
 *   docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
int NEpisodeCur(void)
{
    int i = IlevelCur__Fv();
    if (i == -1)
        return -1;
    return D_003E1C48[i];
}
