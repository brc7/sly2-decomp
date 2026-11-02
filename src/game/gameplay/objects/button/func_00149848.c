#include "m2c_prelude.h"
M2C_UNK CopyAb__FPvT0Ui(s32 *, s32 *, s32);         /* extern */
M2C_UNK Decref__4CRef(s32);                         /* extern */
extern M2C_UNK D_002A2E50;
extern M2C_UNK D_002A2E58;
extern s32 D_002A2E5C;
extern s32 D_002A2E60;
extern M2C_UNK D_002A2E68;
extern f32 D_002A4EE8;
extern M2C_UNK D_002A4F00;
extern u16 D_002A4FD0;
extern M2C_UNK D_002A4FD8;
extern M2C_UNK D_002A4FE0;
extern M2C_UNK D_002A4FE8;
extern f32 D_002BD460;
extern M2C_UNK D_002BD468;
extern f32 D_002BD470;
extern f32 D_002BD474;
extern f32 D_002BD478;
extern f32 D_002BD47C;
extern M2C_UNK D_002BD480;
extern M2C_UNK D_002BD488;
extern M2C_UNK D_002BD490;
extern M2C_UNK D_002BD498;
extern M2C_UNK D_002BD4A0;
extern M2C_UNK D_002BD4B0;
extern s32 D_002BD4C0;
extern void *D_002DD2F8;
extern s32 D_002DD648;
extern u8 D_002DD680;
extern s32 D_002DDE80;
extern s32 D_002DDE84;
extern f32 D_002DDED0;
extern f32 D_002DDF0C;
extern M2C_UNK D_002DDF40;
extern M2C_UNK D_002DEB30;
extern s32 D_002DFC00;
extern s32 D_002E0CB4;
extern s32 D_003DA0B8;
extern M2C_UNK D_003DA4F0;
extern M2C_UNK D_003DD040;
extern s32 D_003DD0E4;
extern M2C_UNK D_003DD490;
extern s32 D_003E134C;
extern s32 D_003E1450;
extern M2C_UNK D_004520A8;

/** @sly2-annot addr=0x149848
 * Purpose: build a CRef from +0x2C0/+0x2C4 (0x200F58 tag 0x2002)
 * Info: proposed name RefGetVolbtnPushState (T2); TU button
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/gameplay-objects.md#button-0x148620-0x149a50
 * @end-sly2-annot */
s32 func_00149848(s32 arg0, s32 arg1) {
    func_00200F58(arg0, M2C_FIELD(arg1, s32 *, 0x2C0), 0x2002, M2C_FIELD(arg1, s32 *, 0x2C4));
    return arg0;
}
