struct BIG { int a[0x5408 / 4]; };
extern struct BIG *D_002DEB4C;
extern struct BIG D_003D4A60;
void ResetRumbleSaveVar(void);
void ApplyGsSettings(struct BIG *p);
/** @sly2-annot addr=0x166008
 * Purpose: Copies the pending GS blob (g_game+0x1C) into g_gs and applies its settings
 * Info: proposed name ApplyPendingGs (T2); TU game
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
void func_00166008(void)
{
    if (D_002DEB4C) {
        D_003D4A60 = *D_002DEB4C;
        D_002DEB4C = 0;
        ResetRumbleSaveVar();
        ApplyGsSettings(&D_003D4A60);
    }
}
