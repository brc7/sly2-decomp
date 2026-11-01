int func_001F4FC8(void *p, int a);
int func_00220498(void *p, int a, int b);
void func_001CC9A8(void *p, int a, int b);
/** @sly2-annot addr=0x144388
 * Purpose: vt61: SO base then let jtthrow (0x220498) and jtgrab hands (+0x900, 0x1CC9A8) adjust the result
 * Info: proposed name STEPPLAYER_FIgnoreIntersection (T1); class STEPPLAYER slot61; TU stepplayer
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r2b_rest
 * See: docs/engine/player.md#11-key-functions,
 *   docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638,
 *   docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
void func_00144388(char *p, int a)
{
    int x = func_001F4FC8(p, a);
    x = func_00220498(p + 0x11B0, a, x);
    func_001CC9A8(p + 0x900, a, x);
}
