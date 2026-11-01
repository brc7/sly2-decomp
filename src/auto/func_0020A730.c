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

/** @sly2-annot addr=0x20A730
 * Purpose: Append guard to record (stamp time and reset state on first)
 * Info: proposed name AddTargetRecordGuard (T1); TU stepguard
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7b_guard+verdicts_followups
 * See: docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
s32 func_0020A730(s32 arg0, s32 arg1) {
    s32 temp_v1;
    void *temp_v0;

    if (M2C_FIELD(arg0, s32 *, 0x28) == 0) {
        M2C_FIELD(arg0, f32 *, 0) = (f32) D_002DDED0;
        func_0020A9A8(arg0, 0);
    }
    temp_v1 = M2C_FIELD(arg0, s32 *, 0x28);
    temp_v0 = arg0 + (temp_v1 * 4);
    M2C_FIELD(temp_v0, s32 *, 0x2C) = arg1;
    M2C_FIELD(arg0, s32 *, 0x28) = (s32) (temp_v1 + 1);
    return (s32) temp_v0;
}
