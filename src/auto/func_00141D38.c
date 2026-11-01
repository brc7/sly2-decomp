#include "m2c_prelude.h"
M2C_UNK func_00141CD0(void *, M2C_UNK);             /* extern */
M2C_UNK func_001928F8(void *, M2C_UNK, M2C_UNK, M2C_UNK); /* extern */
M2C_UNK func_00195A00(void *, M2C_UNK, M2C_UNK, M2C_UNK); /* extern */

/** @sly2-annot addr=0x141D38
 * Purpose: vt100: gadgets 14/21/23 -> store id in jtgrab +0x9BC and jtgrab state 0xB; gadget 12 -> charge MP
 *   and jtair state 0x11
 * Info: proposed name StartStepplayerGadget (T1); class STEPPLAYER slot100; TU stepplayer
 * Notes: mangled dropped (base does not match final name): StartGadget__10STEPPLAYERi
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638,
 *   docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
void func_00141D38(void *arg0, s32 arg1) {
    switch (arg1) {                                 /* irregular */
    case 14:
    case 21:
    case 23:
        M2C_FIELD(arg0, s32 *, 0x9BC) = arg1;
        func_001928F8(arg0 + 0x4C0, 0xB, 0, 0);
        return;
    case 12:
        func_00141CD0(arg0, 0xC);
        func_00195A00(arg0 + 0x2C0, 0x11, 0, 0);
        return;
    }
}
