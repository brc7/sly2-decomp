int IdPauseTopPage(void);
/** @sly2-annot addr=0x1B0108
 * Purpose: Top page is one of the save/load pages
 * Info: proposed name FPauseSavePage (T2); TU menu
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/hud-ui.md#menu-0x1ae928-0x1b0b78
 * @end-sly2-annot */
int func_001B0108(void)
{
    switch (IdPauseTopPage()) {
    case 0x19: case 0x1E: case 0x1F:
        return 1;
    }
    return 0;
}
