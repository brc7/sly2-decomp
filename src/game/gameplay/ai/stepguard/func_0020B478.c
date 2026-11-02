#include "m2c_prelude.h"
extern f32 D_002A88C4;
extern f32 D_002A88C8;
extern f32 D_002A88CC;
extern f32 D_002A8904;
extern f32 D_002C3DB0;
extern f32 D_002C3DB4;
extern f32 D_002C3DB8;
extern f32 D_002C3DBC;
extern f32 D_002C3DE0;
extern f32 D_002C3DE4;
extern M2C_UNK D_002DA9D0;
extern M2C_UNK D_002DAA10;
extern f32 D_002DDED0;
extern M2C_UNK D_002DDF40;
extern M2C_UNK D_003DD490;
extern s32 D_003E148C;

/** @sly2-annot addr=0x20B478
 * Purpose: -- +0xFFC; at 0 SetLoBusyk 0
 * Info: proposed name DecrementStepguardBusy (T2); TU stepguard
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r7b_guard
 * See: docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/guards-ai.md#bhvstk-0x1eec60-0x1ef850,
 *   docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_0020B478(s32 arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = M2C_FIELD(arg0, s32 *, 0xFFC) - 1;
    M2C_FIELD(arg0, s32 *, 0xFFC) = temp_v0;
    if (temp_v0 == 0) {
        SetLoBusyk(arg0, 0);
    }
}
