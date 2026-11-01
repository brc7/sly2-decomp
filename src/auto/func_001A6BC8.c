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

/** @sly2-annot addr=0x1A6BC8
 * Purpose: LO vt39(XF*): SetLoXfFromMat(plo, pxf+0x40)
 * Info: proposed name SetLoXf (T2); class LO slot39; TU lo
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518
 * @end-sly2-annot */
void func_001A6BC8(s32 arg0, s32 arg1) {
    SetLoXfFromMat(arg0, arg1 + 0x40);
}
