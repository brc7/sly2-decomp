extern void *D_003E1AC8;
int func_0013C478(void *p);
void func_00143380(void *p);
void *func_001CB250(void *p, int n);
void func_0013C490(void *p, int n);
void func_0013C560(void *p, int n);
/** @sly2-annot addr=0x1432E0
 * Purpose: If carrying booty id n not already shown: release old, clone template D_003E1AC8 (0x1CB250), set
 *   its id, owner (+0x160), states 6 then 4
 * Info: proposed name SpawnStepplayerBootyMarker (T2); TU stepplayer
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void func_001432E0(char *p, int n)
{
    void *q = *(void **)(p + 0x11A8);
    char *o;
    if (q && func_0013C478(q) == n) return;
    func_00143380(p);
    if (n == -1) return;
    if (!D_003E1AC8) return;
    o = func_001CB250(D_003E1AC8, 1);
    func_0013C490(o, n);
    *(void **)(o + 0x160) = p;
    func_0013C560(o, 6);
    func_0013C560(o, 4);
}
