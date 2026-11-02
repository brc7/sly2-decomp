#include "m2c_prelude.h"
M2C_UNK CopyAb__FPvT0Ui(s32, s32, s32);             /* extern */
M2C_UNK Decref__4CRef(M2C_UNK *);                   /* extern */
extern s128 D_002A2DD0;
extern f32 D_002A886C;
extern M2C_UNK D_002C3D70;
extern M2C_UNK D_002C3D80;
extern f32 D_002C3D90;
extern f32 D_002C3D94;
extern M2C_UNK D_002C3D98;
extern f32 D_002C3DA8;
extern f32 D_002C3DAC;
extern f32 D_002DDED0;
extern M2C_UNK D_002DEB30;
extern M2C_UNK D_003DD490;

/** @sly2-annot addr=0x2081C8
 * Purpose: Release the 3 zap tables
 * Info: proposed name ReleaseStepguardZapReactions (T2); TU stepguard
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_002081C8(s32 arg0, s32 arg1) {
    func_00208178(arg0, arg0 + 0xC10);
    func_00208178(arg0, arg0 + 0x8D0);
    func_00208178(arg0, arg0 + 0xA70);
}
