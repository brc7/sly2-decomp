/** @sly2-annot addr=0x143C78
 * Purpose: vt85: returns a mode code from component states (0 if +0x204, 4 in grab 3/4, 1 in air, 7
 *   standing/hurt-idle, 1 in hurt 4/7/8/12)
 * Info: proposed name STEPPLAYER_VUnk84 (T2); class STEPPLAYER slot85; TU stepplayer
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638,
 *   docs/reference/functions/player.md#jt-0x188488-0x18e540,
 *   docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
int func_00143C78(char *p)
{
    if (*(int *)(p + 0x204)) return 0;
    switch (*(int *)(p + 0x4C8)) {
    case 3:
    case 4:
        return 4;
    }
    if (*(int *)(p + 0x2DC) != -1) return 1;
    switch (*(int *)(p + 0xDF4)) {
    case -1:
    case 0:
    case 6:
        return 7;
    case 4:
    case 7:
    case 8:
    case 12:
        return 1;
    }
    return 0;
}
