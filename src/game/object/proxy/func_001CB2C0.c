#include "m2c_prelude.h"
M2C_UNK Flush__4DMAS(M2C_UNK *);                   /* extern */
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

/** @sly2-annot addr=0x1CB2C0
 * Purpose: PspareChoose(this 2)!=0
 * Info: proposed name FCanAcquireProxy (T1); TU proxy
 * Notes: verifier CORRECT: FCanAcquirePspare -> FCanAcquireProxy
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r5_1AE130+verdicts_tu_r3r5
 * See: docs/engine/object-model.md#9-key-functions,
 *   docs/reference/functions/object-model.md#proxy-0x1c9940-0x1cb908
 * @end-sly2-annot */
s32 func_001CB2C0(s32 arg0, s32 arg1) {
    return func_001CADE0(arg0, 2) != 0;
}
