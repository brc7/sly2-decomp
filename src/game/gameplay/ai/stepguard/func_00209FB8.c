#include "m2c_prelude.h"
M2C_UNK CopyAb__FPvT0Ui(s32, s32, s32);             /* extern */
M2C_UNK func_00129EC8(M2C_UNK *);                   /* extern */
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

/** @sly2-annot addr=0x209FB8
 * Purpose: Flag 0x16 or 0x13: BHV176 op 3
 * Info: proposed name StunStepguardBhv176C (T2); TU stepguard
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_00209FB8(s32 arg0) {
    if ((FStepguardHasSgf(arg0, 0x16) != 0) || (FStepguardHasSgf(arg0, 0x13) != 0)) {
        func_0017A8C8(func_00205458(arg0, 0xB0), 3);
    }
}
