extern float D_002BD260;
extern float D_002BD264;
void func_001441E8(void *p, float f);
/** @sly2-annot addr=0x141DD0
 * Purpose: vt101: while a gadget is held, drain MP at a per-second rate for gadgets 0x1D and 0x2A (DrainMp
 *   0x1441E8)
 * Info: proposed name UpdateStepplayerGadget (T1); class STEPPLAYER slot101; TU stepplayer
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void func_00141DD0(void *p, int *pm)
{
    switch (*pm) {
    case 0x1D:
        func_001441E8(p, D_002BD260);
        break;
    case 0x2A:
        func_001441E8(p, D_002BD264);
        break;
    }
}
