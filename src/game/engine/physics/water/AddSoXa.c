#include "m2c_prelude.h"
/* Warning: missing "jr $ra" in last block of func_0022B040 (initial). */

s32 func_001AE318(s32 *, M2C_UNK);                  /* extern */
extern M2C_UNK D_002A2D00;
extern s128 D_002A2DA0;
extern s128 D_002A2DB0;
extern s128 D_002A2DC0;
extern s128 D_002A2DD0;
extern M2C_UNK D_002BBB60;
extern f32 D_002C4470;
extern f32 D_002C4480;
extern f32 D_002C449C;
extern M2C_UNK D_002C44A0;
extern M2C_UNK D_002C44B0;
extern M2C_UNK D_002C44C0;
extern M2C_UNK D_002C44D0;
extern f32 D_002DDED0;
extern f32 D_002DDED4;
extern M2C_UNK D_002DDF40;
extern M2C_UNK D_002DEB30;
extern M2C_UNK D_003DD490;
extern M2C_UNK D_003E10D0;
extern M2C_UNK D_00443740;

/** @sly2-annot addr=0x229710
 * Purpose: push XA on SO XA list (0x138) then 0x1F5078 (wake)
 * Info: proposed name AddSoXa (T1); TU water
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r8_21BB88+verify_side
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#water-0x228b48-0x229ef0
 * @end-sly2-annot */
void AddSoXa(s32 arg0, s32 arg1) {
    M2C_FIELD(arg1, s32 *, 8) = (s32) M2C_FIELD(arg0, s32 *, 0x138);
    M2C_FIELD(arg0, s32 *, 0x138) = arg1;
    func_001F5078(arg0);
}
