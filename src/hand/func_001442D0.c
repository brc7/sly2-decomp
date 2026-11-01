void func_00205478(void *p, int a, int b);
void func_00143EE0(void *p, int a, int b, int c);
void func_0019CD38(void *p, int a);
/** @sly2-annot addr=0x1442D0
 * Purpose: vt83 (STEP VUnk82(grf,fCount)): base 0x205478; when counting and +0x12F4 goes 0->1 play script
 *   anim 0x60B and reset jtmode
 * Info: proposed name STEPPLAYER_VUnk82 (T2); class STEPPLAYER slot83; TU stepplayer
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void func_001442D0(char *p, int a, int b)
{
    func_00205478(p, a, b);
    if (b) {
        if (++*(int *)(p + 0x12F4) == 1) {
            func_00143EE0(p, 0x60B, 9, 5);
            func_0019CD38(p + 0xF00, -1);
        }
    }
}
