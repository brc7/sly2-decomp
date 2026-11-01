#include "m2c_prelude.h"
/* Warning: missing "jr $ra" in last block of func_001A50E8 (initial). */

M2C_UNK CopyAb__FPvT0Ui(s32, s32, s32);             /* extern */
M2C_UNK func_00129EC8(void *);                      /* extern */
extern f32 D_002A6A40;
extern M2C_UNK D_002C44F0;
extern f32 D_002DDED0;
extern M2C_UNK D_002DEB30;
extern M2C_UNK D_003DD490;
extern M2C_UNK D_003DD4A0;
extern M2C_UNK D_003DD4BC;
extern s32 D_003E139C;
extern M2C_UNK D_004520D0;
extern M2C_UNK D_00452148;

/** @sly2-annot addr=0x1A50F8
 * Purpose: Queue kind 5
 * Info: proposed name AddBootMemcardStep (T2); TU tu_1A4E08
 * Notes: names_v2 v1 proposed SetLightShadowStrength__FP5LIGHTf (0.000)
 * Confidence: low (draft) [T2 0.30] - evidence: static naming pass; sources: r4b_rest
 * See: docs/reference/functions/world-sw.md#tu_1a4e08-0x1a4e08-0x1a5270
 * @end-sly2-annot */
void func_001A50F8(s32 arg0, s32 arg1) {
    func_001A4FB0(arg0, 5);
}
