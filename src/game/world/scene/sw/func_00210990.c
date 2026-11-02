struct E { char pad[0x40]; };
extern struct E *D_003E1C40;
/** @sly2-annot addr=0x210990
 * Purpose: &g_arecLevel[ilevel] (NULL for -1)
 * Info: proposed name PrecLevelFile (T2); TU sw
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
struct E *func_00210990(int i)
{
    if (i == -1)
        return 0;
    return &D_003E1C40[i];
}
