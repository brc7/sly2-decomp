#include "m2c_prelude.h"
M2C_UNK CopyAb__FPvT0Ui(s32, s32, s32);             /* extern */
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

/** @sly2-annot addr=0x221CC0
 * Purpose: clears tn/ppo (0x10,0x14) then base 0x14FCE8
 * Info: proposed name DeactivateCptn (T1); TU tn
 * Notes: agent qualifier: CPTN vt slot3
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/camera.md#tn-0x220d40-0x223f70
 * @end-sly2-annot */
void DeactivateCptn(s32 arg0) {
    M2C_FIELD(arg0, s32 *, 0x10) = 0;
    M2C_FIELD(arg0, s32 *, 0x14) = 0;
    func_0014FCE8(arg0);
}
