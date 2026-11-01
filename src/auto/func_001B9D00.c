#include "m2c_prelude.h"
s32 func_001AE318(s32 *);                           /* extern */
extern M2C_UNK D_002A2D30;
extern s128 D_002A2DD0;
extern f32 D_002A7620;
extern M2C_UNK D_002BBB60;
extern s128 D_002C09D0;
extern f32 D_002C09F0;
extern M2C_UNK D_002C09F8;
extern M2C_UNK D_002C0A20;
extern f32 D_002C0A30;
extern f32 D_002C0A34;
extern f32 D_002C0A38;
extern f32 D_002C0A3C;
extern f32 D_002C0A40;
extern M2C_UNK D_002C0A50;
extern s128 D_002C0A80;
extern M2C_UNK D_002C3C60;
extern f32 D_002DDED0;
extern M2C_UNK D_002DEB30;
extern s32 D_002E1E40;
extern M2C_UNK D_003DD490;
extern s32 D_003E1444;

/** @sly2-annot addr=0x1B9D00
 * Purpose: Splice-callable: clear alternate flag x13E8 and SetMurrayState(3)
 * Info: proposed name StartMurrayFlop (T2); TU murray
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
void func_001B9D00(s32 arg0, s32 arg1, s32 arg2) {
    M2C_FIELD(arg0, s32 *, 0x13E8) = 0;
    func_001B6078(arg0, 3, arg2);
}
