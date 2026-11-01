#include "m2c_prelude.h"
M2C_UNK IncrementSwHandsOff(M2C_UNK *, M2C_UNK);          /* extern */
M2C_UNK DecrementSwHandsOff(M2C_UNK *, M2C_UNK);          /* extern */
extern f32 D_002DDED0;
extern M2C_UNK D_003DD490;

/** @sly2-annot addr=0x153128
 * Purpose: Option setter: state(+0x50) change; entering 1 = IncrementSwHandsOff(g_sw,3), leaving 1 =
 *   Decrement; stamps time, resets cue index
 * Info: proposed name SetRhythmMgrState (T1); TU rhythm
 * Confidence: mid (draft) [T1 0.55] - evidence: semantic review; sources: r2_131F70
 * See: docs/reference/functions/gameplay-objects.md#rhythm-0x1520c8-0x1539a8
 * @end-sly2-annot */
void SetRhythmMgrState(void *arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = M2C_FIELD(arg0, s32 *, 0x50);
    if (temp_v0 != arg1) {
        if (temp_v0 == 1) {
            DecrementSwHandsOff(&D_003DD490, 3);
        }
        M2C_FIELD(arg0, s32 *, 0x50) = arg1;
        M2C_FIELD(arg0, f32 *, 0x54) = (f32) D_002DDED0;
        M2C_FIELD(arg0, s32 *, 0x1C0) = 0;
        if (arg1 == 1) {
            IncrementSwHandsOff(&D_003DD490, 3);
        }
    }
}
