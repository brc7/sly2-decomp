#include "m2c_prelude.h"
M2C_UNK SetRumbleRums__FP6RUMBLE4RUMS(void *, M2C_UNK); /* extern */
s32 func_001D55B8();                                /* extern */

/** @sly2-annot addr=0x1D5340
 * Purpose: Start a rumble pattern for dt if pad active and FRumbleEnabled
 * Info: proposed name TriggerRumbleRumpat__FP6RUMBLEP6RUMPATf (EXISTING); TU rumble
 * Confidence: high [EXISTING 0.90] - evidence: pre-existing symbol in symbol_addrs.txt; sources: r6_1D5150
 * See: docs/reference/functions/main-loop-startup.md#rumble-0x1d5150-0x1d57a8
 * @end-sly2-annot */
void TriggerRumbleRumpat__FP6RUMBLEP6RUMPATf(void *arg0, void *arg1, f32 fparg0) {
    if ((M2C_FIELD(arg0, s32 *, 8) != 0) && (M2C_FIELD(arg1, s32 *, 0) > 0) && (func_001D55B8() != 0)) {
        M2C_FIELD(arg0, f32 *, 0x14) = fparg0;
        M2C_FIELD(arg0, void **, 0xC) = arg1;
        M2C_FIELD(arg0, s32 *, 0x10) = 0;
        M2C_FIELD(arg0, f32 *, 0x18) = (f32) M2C_FIELD(arg1, f32 *, 8);
        M2C_FIELD(arg0, s32 *, 8) = -1;
        SetRumbleRums__FP6RUMBLE4RUMS(arg0, 2);
    }
}
