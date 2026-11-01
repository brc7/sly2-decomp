#include "m2c_prelude.h"
M2C_UNK CopyAb__FPvT0Ui(s128 *, s128 *, s8, s128);  /* extern */
extern M2C_UNK D_0025C1C0;
extern M2C_UNK D_0025E040;
extern M2C_UNK D_0025E090;
extern M2C_UNK D_0025E668;
extern M2C_UNK D_0025E680;
extern M2C_UNK D_00262320;
extern M2C_UNK D_002A2D30;
extern M2C_UNK D_002A3828;
extern f32 D_002A5270;
extern f32 D_002A52A0;
extern M2C_UNK D_002BE540;
extern M2C_UNK D_002BE558;
extern s32 D_002BE580;
extern M2C_UNK D_002DDF40;
extern M2C_UNK D_002DEA28;
extern s32 D_002DEA4C;
extern M2C_UNK D_002DEA50;
extern M2C_UNK D_002DEB30;
extern s32 D_002DEB4C;
extern s32 D_002DEB54;
extern M2C_UNK D_002DEB60;
extern M2C_UNK D_002DEB70;
extern s32 D_002DEB74;
extern M2C_UNK D_002DF800;
extern M2C_UNK D_002DF940;
extern M2C_UNK D_002DFA80;
extern M2C_UNK D_003D4A60;
extern M2C_UNK D_003DA530;
extern s32 D_003DA790;
extern s32 D_003DCFA0;
extern s32 D_003DCFA8;
extern M2C_UNK D_003DD490;
extern s32 D_003E11EC;
extern s32 D_003E12C8;
extern s32 D_003E12D0;
extern s64 D_003E1330;
extern M2C_UNK D_10;
extern M2C_UNK D_1910;
extern M2C_UNK D_1A68;
extern M2C_UNK D_1AA0;
extern M2C_UNK D_1C48;
extern M2C_UNK D_2E20;
extern M2C_UNK D_2E68;
extern M2C_UNK D_2EB0;
extern M2C_UNK D_2EE8;
extern M2C_UNK D_2EF8;
extern M2C_UNK D_2F80;
extern M2C_UNK D_2FC8;
extern M2C_UNK D_3358;
extern M2C_UNK D_3628;
extern M2C_UNK D_37A8;
extern M2C_UNK D_3AB0;

/** @sly2-annot addr=0x166A50
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/rendering.md#gifs-0x166920-0x167310
 * @end-sly2-annot */
s32 CheckReg__4GIFSii(void *arg0) {
    s32 temp_v0;

    temp_v0 = M2C_FIELD(arg0, s32 *, 0x24) + 1;
    M2C_FIELD(arg0, s32 *, 0x24) = temp_v0;
    return temp_v0;
}
