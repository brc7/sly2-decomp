void func_001C4228(void *p, int a, int b);
void func_00144188(void *p, int a);
int func_001441A0(void *p);
void func_001441C0(void *p, int a);
/** @sly2-annot addr=0x144268
 * Purpose: vt81: PO base then hp += dhp, mp += dmp (clamped)
 * Info: proposed name AddStepplayerHpMp (T1); class STEPPLAYER slot81; TU stepplayer
 * Notes: mangled dropped (base does not match final name): AddHpMp__10STEPPLAYERii
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void func_00144268(char *p, int a, int b)
{
    func_001C4228(p, a, b);
    func_00144188(p, **(int **)(p + 0xE00) + a);
    func_001441C0(p, func_001441A0(p) + b);
}
