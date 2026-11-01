#include "m2c_prelude.h"
M2C_UNK func_00117ED0(s32, s32);                    /* extern */
extern f32 D_002C3B90;
extern M2C_UNK D_002C44F0;
extern void *D_002DD2FC;
extern M2C_UNK D_002DDED0;
extern f32 D_002DDED4;
extern M2C_UNK D_003DD490;
extern M2C_UNK D_003E11AC;
extern M2C_UNK D_003E11B8;
extern M2C_UNK D_003E12D4;

/** @sly2-annot addr=0x1F3590
 * Purpose: SO vt64: SOPHYS gravity vector (+0x80) = (0,0,-1750), both XF modes (+0xA0/+0xC0) = 3 (fixed)
 * Info: proposed name ResetSoPhys (T2); class SO slot64; TU so
 * Notes: names_v2 C proposed ConstrFromCnstr__F5CNSTRP2CTP6VECTOR (0.438)
 * Confidence: low (draft) [T2 0.35] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
s32 func_001F3590(s32 arg0) {
    void *temp_v0;
    void *temp_v0_2;
    void *temp_v0_3;

    temp_v0_2 = M2C_FIELD(arg0, void **, 0xE8);
    temp_v0_3 = temp_v0_2 + 0x80;
    M2C_FIELD(temp_v0_2, s32 *, 0x80) = 0;
    M2C_FIELD(temp_v0_3, f32 *, 8) = -1750.0f;
    M2C_FIELD(temp_v0_3, s32 *, 4) = 0;
    M2C_FIELD(M2C_FIELD(arg0, void **, 0xE8), s32 *, 0xC0) = 3;
    temp_v0 = M2C_FIELD(arg0, void **, 0xE8);
    M2C_FIELD(temp_v0, s32 *, 0xA0) = 3;
    return (s32) temp_v0;
}
