extern unsigned char D_002BE328[];
extern void *D_002DE9F0[];
/** @sly2-annot addr=0x15EFA0
 * Purpose: Maps a font-escape character (table D_002BE328, 2 entries) to one of the two registered fonts
 * Info: proposed name PfontFromCh (T2); TU font
 * Notes: names_v2 B proposed CleanupDraw__5CFontP8CTextBoxP4GIFS (0.856)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r3_1539A8
 * See: docs/reference/functions/hud-ui.md#font-0x15ef00-0x1603b8
 * @end-sly2-annot */
void *func_0015EFA0(unsigned char ch)
{
    int i;
    for (i = 0; i < 2; i++)
        if (ch == D_002BE328[i])
            break;
    if (i < 2)
        return D_002DE9F0[i];
    return 0;
}
