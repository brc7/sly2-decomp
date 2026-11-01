#include "m2c_prelude.h"
M2C_UNK func_0013A9E8();                            /* extern */

/** @sly2-annot addr=0x1B1DB8
 * Purpose: MISSILE::Init (vt7): InitBomb; fFollowTrajectory(+0x264)=1; +0x260=800; +0x26C=1
 * Info: proposed name InitMissile (T1); class MISSILE slot7; TU missile
 * Confidence: high [T1 0.90] - evidence: static analysis; sources: r5_1AE130
 * See: docs/engine/gameplay-objects.md#gameplay-object-classes, docs/engine/gameplay-objects.md#source-files,
 *   docs/engine/gameplay-objects.md#key-functions
 * @end-sly2-annot */
void func_001B1DB8(void *arg0) {
    func_0013A9E8();
    M2C_FIELD(arg0, s32 *, 0x264) = 1;
    M2C_FIELD(arg0, f32 *, 0x260) = 800.0f;
    M2C_FIELD(arg0, f32 *, 0x26C) = 1.0f;
}
