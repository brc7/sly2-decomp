void func_00205588(void *p, int a, int b);
void func_00143FF8(void *p);
/** @sly2-annot addr=0x144338
 * Purpose: vt84 (STEP VUnk83(grf,fCount)): base 0x205588; when +0x12F4 drops to 0 stop the script anim
 * Info: proposed name STEPPLAYER_VUnk83 (T2); class STEPPLAYER slot84; TU stepplayer
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void func_00144338(char *p, int a, int b)
{
    func_00205588(p, a, b);
    if (b) {
        if (--*(int *)(p + 0x12F4) == 0) func_00143FF8(p);
    }
}
