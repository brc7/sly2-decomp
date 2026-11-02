#include "m2c_prelude.h"
extern s128 D_002A2DC0;
extern s128 D_002A2DD0;
extern f32 D_002A4AF0;
extern s128 D_002BC920;
extern s128 D_002BC970;
extern s128 D_002BC980;
extern f32 D_002BC998;
extern f32 D_002BC99C;
extern f32 D_002BC9A0;
extern M2C_UNK D_002BC9A8;
extern f32 D_002BC9B4;
extern s32 D_002BC9B8;
extern M2C_UNK D_002C44E0;
extern s32 D_002DD5BC;
extern f32 D_002DDED0;
extern M2C_UNK D_002DDF40;
extern void *D_002F7900;
extern M2C_UNK D_003DD490;

/** @sly2-annot addr=0x1374E0
 * Purpose: arg1+0x94 = const
 * Info: proposed name SetJtContactParam (T2); TU bentley
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#bentley-0x134648-0x1378d8
 * @end-sly2-annot */
void func_001374E0(s32 arg0, s32 arg1) {
    M2C_FIELD(arg1, f32 *, 0x94) = (f32) D_002BC9B4;
}
