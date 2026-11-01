void func_001F4FA8(void *p);
void func_001991A0(void *p);
void func_00193DE8(void *p);
void func_0019E950(void *p);
/** @sly2-annot addr=0x140D00
 * Purpose: vt59: SO base 0x1F4FA8 then per active component (jtair +0x2C0, jtgrab +0x4C0, jtpipe +0xF50 with
 *   state!=-1) call its update hook (0x1991A0, UpdateJtInternalXpsHide 0x193DE8, 0x19E950)
 * Info: proposed name STEPPLAYER_VUnk58 (T2); class STEPPLAYER slot59; TU stepplayer
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void func_00140D00(char *p)
{
    func_001F4FA8(p);
    if (*(int *)(p + 0x2DC) != -1) func_001991A0(p + 0x2C0);
    if (*(int *)(p + 0x4CC) != -1) func_00193DE8(p + 0x4C0);
    if (*(int *)(p + 0xF54) != -1) func_0019E950(p + 0xF50);
}
