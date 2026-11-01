#include "m2c_prelude.h"
M2C_UNK func_001574A0(M2C_UNK *);                   /* extern */
s32 func_001AE318(s32 *, s32, s32);                 /* extern */
extern s128 D_002A2DD0;
extern f32 D_002A77D0;
extern f32 D_002A77D4;
extern M2C_UNK D_002AC880;
extern f32 D_002C1EC0;
extern f32 D_002C1EC4;
extern f32 D_002C1EC8;
extern f32 D_002C1ECC;
extern s32 D_002C1ED0;
extern s128 D_002C1EE0;
extern f32 D_002C1EF0;
extern f32 D_002C1EF4;
extern f32 D_002C1F20;
extern M2C_UNK D_002C1F28;
extern s32 D_002C1F30;
extern s32 D_002C1F34;
extern M2C_UNK D_002D77D8;
extern M2C_UNK D_002D9978;
extern s32 *D_002DD2FC;
extern f32 D_002DDED0;
extern M2C_UNK D_002DDF40;
extern M2C_UNK D_002DEA00;
extern M2C_UNK D_002DEA28;
extern s32 D_002F7900;
extern M2C_UNK D_003B3980;
extern s32 D_003D4A50;
extern M2C_UNK D_003DA0F0;
extern M2C_UNK D_003DD490;
extern M2C_UNK D_003DD4BC;
extern s32 D_003E115C;
extern s32 D_003E1398;

/** @sly2-annot addr=0x1CCA20
 * Purpose: SW callback wrapper calling UpdateJtHands
 * Info: proposed name UpdateJtHandsCallback (T2); TU jthands
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/player.md#jthands-0x1cc158-0x1cd5f0
 * @end-sly2-annot */
void func_001CCA20(s32 arg0, s32 arg1) {
    func_001CC2E8(arg1);
}
