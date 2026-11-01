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

/** @sly2-annot addr=0x1F7B88
 * Purpose: return -(+0x149 byte)
 * Info: proposed name GetSoDhp1 (T2); TU so
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
s32 func_001F7B88(s32 arg0) {
    return (s32) -M2C_FIELD(arg0, s8 *, 0x149);
}
