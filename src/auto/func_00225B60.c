#include "m2c_prelude.h"
M2C_UNK func_001F36E0();                            /* extern */
M2C_UNK func_00225BA0(s32);                         /* extern */
M2C_UNK func_00225E98(s32);                         /* extern */
M2C_UNK func_00226250(s32);                         /* extern */

/** @sly2-annot addr=0x225B60
 * Purpose: SO update then aim (0x225BA0), zoom (0x225E98), fire (0x226250)
 * Info: proposed name UpdateTurret (T1); class TURRET slot16; TU turret
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/gameplay-objects.md#turret-0x2259d8-0x226550
 * @end-sly2-annot */
void func_00225B60(s32 arg0) {
    func_001F36E0();
    func_00225BA0(arg0);
    func_00225E98(arg0);
    func_00226250(arg0);
}
