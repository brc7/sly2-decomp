extern void *D_003D4A54;
int FIsPoCur(void *p);
int func_001A2230(void *p, int a);
/** @sly2-annot addr=0x140ED8
 * Purpose: vt76: invulnerable if jtmode state==1, jtair state 0x15, the D_003D4A54 PO is current, else jthurt
 *   check 0x1A2230(zpk)
 * Info: proposed name STEPPLAYER_FInvulnerable (T1); class STEPPLAYER slot76; TU stepplayer
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638,
 *   docs/reference/functions/player.md#hurt-0x1a0b18-0x1a2bd0,
 *   docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
int STEPPLAYER_FInvulnerable(char *p, int a)
{
    if (*(int *)(p + 0xF20) == 1) return 1;
    if (*(int *)(p + 0x2DC) == 0x15) return 1;
    if (D_003D4A54 && FIsPoCur(D_003D4A54)) return 1;
    return func_001A2230(p + 0xDF0, a);
}
