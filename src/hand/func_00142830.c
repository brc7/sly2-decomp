void func_001427C8(void *p, int a, int b);
/** @sly2-annot addr=0x142830
 * Purpose: Splice: SetStepplayerLeanTarget then +0x1100=1 (locked)
 * Info: proposed name ForceStepplayerLeanTarget (T1); TU stepplayer
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void func_00142830(char *p, int a, int b)
{
    func_001427C8(p, a, b);
    *(int *)(p + 0x1100) = 1;
}
