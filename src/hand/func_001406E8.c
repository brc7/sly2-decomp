void func_001C3ED8(void *p, int n);
void func_001F82B8(void *p, int a, int b);
void func_001EECD0(void *p);
/** @sly2-annot addr=0x1406E8
 * Purpose: vt79: PO SetPoCtlState, then 0x1F82B8(p, n==1?0:-1, 0); on becoming current (n==1) clear the
 *   message list +0x26C (0x1EECD0)
 * Info: proposed name SetStepplayerCtlState (T1); class STEPPLAYER slot79; TU stepplayer
 * Notes: mangled dropped (base does not match final name): SetCtlState__10STEPPLAYERi
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void func_001406E8(char *p, int n)
{
    func_001C3ED8(p, n);
    func_001F82B8(p, n == 1 ? 0 : -1, 0);
    if (n == 1) func_001EECD0(p + 0x26C);
}
