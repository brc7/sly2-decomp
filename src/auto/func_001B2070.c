#include "m2c_prelude.h"
M2C_UNK func_001B1DB8();                            /* extern */

/** @sly2-annot addr=0x1B2070
 * Purpose: ACCMISS::Init: InitMissile; svFast(+0x270)=2000, svTarget(+0x274)=5000, fFollowTrajectory=0
 * Info: proposed name InitAccmiss (T1); class ACCMISS slot7; TU missile
 * Notes: names_v2 v1 proposed FUN_0018e480__Fi (0.000)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r5_1AE130
 * See: docs/engine/gameplay-objects.md#key-functions,
 *   docs/reference/functions/gameplay-objects.md#missile-0x1b1db8-0x1b2820
 * @end-sly2-annot */
void func_001B2070(void *arg0) {
    func_001B1DB8();
    M2C_FIELD(arg0, f32 *, 0x270) = 2000.0f;
    M2C_FIELD(arg0, f32 *, 0x274) = 5000.0f;
    M2C_FIELD(arg0, s32 *, 0x264) = 0;
}
