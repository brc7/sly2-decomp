#include "m2c_prelude.h"
M2C_UNK func_001B1DB8();                            /* extern */

/** @sly2-annot addr=0x1B21F0
 * Purpose: CID-82 missile Init: InitMissile; +0x274=1500
 * Info: proposed name InitSplmiss (T1); class CID82 slot7; TU missile
 * Notes: names_v2 v1 proposed FUN_0018e4c0__Fi (0.000)
 * Confidence: mid (draft) [T1 0.50] - evidence: static naming pass; sources: r5_1AE130
 * See: docs/reference/functions/gameplay-objects.md#missile-0x1b1db8-0x1b2820
 * @end-sly2-annot */
void func_001B21F0(void *arg0) {
    func_001B1DB8();
    M2C_FIELD(arg0, f32 *, 0x274) = 1500.0f;
}
