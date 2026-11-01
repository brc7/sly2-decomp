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

/** @sly2-annot addr=0x22B7A0
 * Purpose: append arg1 to DL at wr+0x8F0 and set arg1->wr
 * Info: proposed name AddWrEntry (T2); TU wr
 * Notes: names_v2 C proposed InitWr__FP2WR (0.196)
 * Confidence: low (draft) [T2 0.30] - evidence: static naming pass; sources: r8_21BB88
 * See: docs/reference/functions/animation.md#wr-0x22b720-0x22c818
 * @end-sly2-annot */
void func_0022B7A0(s32 arg0, s32 arg1) {
    func_0011A070(arg0 + 0x8F0, arg1);
    M2C_FIELD(arg1, s32 *, 0xC) = arg0;
}
