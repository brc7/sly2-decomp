#include "m2c_prelude.h"
M2C_UNK CopyAb__FPvT0Ui(M2C_UNK, f32 *, s32);       /* extern */
f32 *func_001AE318(s32 *, s32);                     /* extern */
extern f32 D_002A4FF0;
extern f32 D_002A4FF4;
extern f32 D_002A4FF8;
extern f32 D_002A4FFC;
extern s128 D_002A5000;
extern M2C_UNK D_002BBB60;
extern M2C_UNK D_002BD810;
extern f32 D_002BD81C;
extern f32 D_002BD820;
extern f32 D_002BD824;
extern f32 D_002BD850;
extern f32 D_002BD854;
extern M2C_UNK D_002BD860;
extern M2C_UNK D_002BD8B0;
extern f32 D_002BD8D4;
extern f32 D_002BD8D8;
extern f32 D_002BD8DC;
extern M2C_UNK D_002BD8E0;
extern f32 D_002DDED0;
extern f32 D_002DDED4;
extern f32 D_002DDEE0;
extern M2C_UNK D_002DDF30;
extern s32 D_002DDF34;
extern M2C_UNK D_002DDF40;
extern s32 D_002DFBC0;
extern M2C_UNK D_002DFC00;
extern M2C_UNK D_002E0D00;
extern M2C_UNK D_003DD490;

/** @sly2-annot addr=0x14F030
 * Purpose: Filter callback: true if SO has camera-BSP nodes (+0x70 count > 0)
 * Info: proposed name FFilterCamera (T1); TU cm
 * Confidence: mid (draft) [T1 0.60] - evidence: semantic review; sources: r2_131F70
 * See: docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
s32 FFilterCamera(s32 arg0, s32 arg1) {
    return M2C_FIELD(arg1, s32 *, 0x70) > 0;
}
