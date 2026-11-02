#include "m2c_prelude.h"
M2C_UNK CopyAb__FPvT0Ui(s32, s32, s32);             /* extern */
M2C_UNK Read__18CBinaryInputStreamiPv(s32, s32, s32);               /* extern */
M2C_UNK func_001291A8(s32, s32 *);                  /* extern */
M2C_UNK Decref__4CRef(s32 *);                       /* extern */
s32 func_001AE318(void *, s32, s32, s32);           /* extern */
s16 atoi(s32);                             /* extern */
s32 fptoui(f32);                             /* extern */
extern M2C_UNK D_002A78F0;
extern M2C_UNK D_002A78F8;
extern f32 D_002A7984;
extern M2C_UNK D_002A79A0;
extern M2C_UNK D_002A79A8;
extern M2C_UNK D_002A79B8;
extern f32 D_002A79C8;
extern M2C_UNK D_002AA6A1;
extern M2C_UNK D_002BCD00;
extern M2C_UNK D_002C2D90;
extern s32 D_002C2DC0;
extern f32 D_002C3254;
extern s32 D_002C3258;
extern f32 D_002C3264;
extern s32 D_002C3268;
extern f32 D_002C3274;
extern f32 D_002C3278;
extern f32 D_002C327C;
extern M2C_UNK D_002D85E8;
extern void *D_002DD2FC;
extern s32 D_002DD318;
extern M2C_UNK D_002DD590;
extern M2C_UNK D_002DDEC0;
extern f32 D_002DDED0;
extern f32 D_002DDF0C;
extern M2C_UNK D_002DEB30;
extern M2C_UNK D_002DEB80;
extern s32 D_002DFBC0;
extern s32 D_002DFBC4;
extern M2C_UNK D_002DFC00;
extern M2C_UNK D_002E0D00;
extern M2C_UNK D_002E1E60;
extern M2C_UNK D_002E6550;
extern M2C_UNK D_002E65A0;
extern M2C_UNK D_003D4A60;
extern M2C_UNK D_003D9E80;
extern M2C_UNK D_003D9ED0;
extern M2C_UNK D_003DA0C0;
extern M2C_UNK D_003DA0F0;
extern M2C_UNK D_003DA150;
extern M2C_UNK D_003DA3C0;
extern M2C_UNK D_003DA4F0;
extern M2C_UNK D_003DA530;
extern M2C_UNK D_003DD040;
extern s32 D_003DD050;
extern M2C_UNK D_003DD490;
extern M2C_UNK D_003E0EA8;
extern M2C_UNK D_00443740;

/** @sly2-annot addr=0x1DA9E8
 * Purpose: SetScreenMode(-1,-1)
 * Info: proposed name CloseScreen (T2); TU screen
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void func_001DA9E8(s32 arg0, s32 arg1, s32 arg2) {
    func_001DA950(arg0, -1, -1);
}
