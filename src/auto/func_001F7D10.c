#include "m2c_prelude.h"
extern s128 D_002A2DC0;
extern s128 D_002A2DD0;
extern f32 D_002A8280;
extern f32 D_002A8284;
extern M2C_UNK D_002CA280;
extern M2C_UNK D_002CA2C0;
extern s32 D_002DD2E0;
extern s32 D_002DD2E4;
extern s32 D_002DD2E8;
extern s32 D_002DD2EC;
extern s32 D_002DD2F0;
extern s32 D_002DD2F4;
extern M2C_UNK D_002DDED0;
extern M2C_UNK D_003DD020;
extern M2C_UNK D_003DD490;
extern s32 D_003E12EC;
extern void *D_003E145C;
extern M2C_UNK D_003E15CC;
extern M2C_UNK D_004520A8;
extern M2C_UNK D_004520D0;
extern M2C_UNK D_004520F8;
extern M2C_UNK D_00452120;
extern M2C_UNK D_00452148;
extern M2C_UNK D_00452170;

/** @sly2-annot addr=0x1F7D10
 * Purpose: alpha<=0: Remove (vt9) unless class 0x69; else Add (vt8); store +0x12C
 * Info: proposed name SetSoAlpha (T1); TU so
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F7D10(s32 arg0, s32 arg1, f32 fparg0) {
    void *temp_v0;
    void *temp_v0_2;

    if (fparg0 <= 0.0f) {
        if (func_00132B10(arg0, 0x69) == 0) {
            temp_v0 = M2C_FIELD(arg0, void **, 0xC);
            M2C_FIELD(temp_v0, void (**)(s32), 0x4C)(arg0 + M2C_FIELD(temp_v0, s16 *, 0x48));
        }
    } else {
        temp_v0_2 = M2C_FIELD(arg0, void **, 0xC);
        M2C_FIELD(temp_v0_2, void (**)(s32), 0x44)(arg0 + M2C_FIELD(temp_v0_2, s16 *, 0x40));
    }
    M2C_FIELD(arg0, f32 *, 0x12C) = fparg0;
}
