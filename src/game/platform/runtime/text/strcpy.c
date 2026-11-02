#include "m2c_prelude.h"
void CopyAb__FPvT0Ui(s32, s32, s32);             /* extern */
M2C_UNK _$_5OSTRM(void *, M2C_UNK);                 /* extern */
s32 func_001AE318(s32 *);                           /* extern */
extern s128 D_002A2DA0;
extern s128 D_002A2DC0;
extern s128 D_002A2DD0;
extern f32 D_002A96A0;
extern f32 D_002A96E0;
extern f32 D_002A96E4;
extern f32 D_002A96F8;
extern f32 D_002A96FC;
extern f32 D_002A9700;
extern M2C_UNK D_002AA6A1;
extern M2C_UNK D_002BBB60;
extern f32 D_002C40E4;
extern M2C_UNK D_002C40F0;
extern f32 D_002C4100;
extern f32 D_002C4104;
extern f32 D_002C4110;
extern f32 D_002C4114;
extern s32 D_002C4118;
extern s32 D_002C4120;
extern M2C_UNK D_002C41A8;
extern M2C_UNK D_002C41B0;
extern s128 D_002C41C0;
extern f32 D_002C41D8;
extern f32 D_002C41DC;
extern f32 D_002C41E0;
extern f32 D_002C41E4;
extern f32 D_002C41E8;
extern f32 D_002C41EC;
extern M2C_UNK D_002C41F0;
extern M2C_UNK D_002C4200;
extern M2C_UNK D_002C4208;
extern M2C_UNK D_002C4220;
extern M2C_UNK D_002C4230;
extern M2C_UNK D_002C4240;
extern M2C_UNK D_002C4250;
extern M2C_UNK D_002C4260;
extern f32 D_002C426C;
extern f32 D_002C4270;
extern f32 D_002C4274;
extern f32 D_002C4278;
extern f32 D_002C427C;
extern f32 D_002C4280;
extern s32 D_002DD5BC;
extern f32 D_002DDED0;
extern M2C_UNK D_002DDF40;
extern M2C_UNK D_002DE220;
extern s32 D_002DE240;
extern M2C_UNK D_002DEB30;
extern s32 D_002DFBC4;
extern M2C_UNK D_002E5270;
extern M2C_UNK D_003DD490;
extern M2C_UNK D_004425D0;
extern M2C_UNK D_004425F0;

/** @sly2-annot addr=0x21ED58
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#text-0x21d638-0x21efd0
 * @end-sly2-annot */
s32 strcpy(s32 arg0, s32 arg1) {
    CopyAb__FPvT0Ui(arg0, arg1, strlen(arg1) + 1);
    return arg0;
}
