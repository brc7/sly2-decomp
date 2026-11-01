/* only diff: original assembler padded a backward branch (to a jr) with a nop at 0x14443C; asfix.py does not pad when a jump is in between. Code otherwise identical. */
struct Q { char pad[0xC]; int c; int n; };
/** @sly2-annot addr=0x144418
 * Purpose: vt86 (STEP VUnk85(ZPR*)->int): 0 if damage below hp max or zap kind in {0,1,0x80000,0x100000},
 *   else 1
 * Info: proposed name STEPPLAYER_VUnk85 (T2); class STEPPLAYER slot86; TU stepplayer
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
int func_00144418(char *p, struct Q *q)
{
    if (-q->n < *(int *)(p + 0xDFC)) return 0;
    switch (q->c) {
    case 0:
    case 1:
    case 0x80000:
    case 0x100000:
        return 0;
    }
    return 1;
}
