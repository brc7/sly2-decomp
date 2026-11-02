#include "m2c_prelude.h"
M2C_UNK func_00117968(s32, void *, s32);            /* extern */
M2C_UNK func_00129EC8(void *);                      /* extern */
s32 func_001AE318(s32 *, s32, s32 *, M2C_UNK);      /* extern */
extern s128 D_002A2DD0;
extern f32 D_002A97A8;
extern M2C_UNK D_002A97F0;
extern M2C_UNK D_002A97F8;
extern M2C_UNK D_002A9878;
extern M2C_UNK32 D_002A9880;
extern M2C_UNK D_002BBB60;
extern f32 D_002C42E0;
extern s32 D_002C42E8;
extern M2C_UNK D_002C4300;
extern M2C_UNK D_002C4320;
extern f32 D_002C4334;
extern M2C_UNK D_002C4338;
extern M2C_UNK D_002C4368;
extern M2C_UNK D_002C4398;
extern f32 D_002C43B0;
extern M2C_UNK D_002C43C0;
extern M2C_UNK D_002C43D0;
extern f32 D_002C43F4;
extern M2C_UNK D_002C4408;
extern M2C_UNK D_002C4418;
extern M2C_UNK D_002C4420;
extern M2C_UNK D_002C4430;
extern M2C_UNK D_002CE488;
extern void **D_002DD2FC;
extern f32 D_002DDED0;
extern f32 D_002DDED4;
extern M2C_UNK D_002DDF40;
extern f32 D_002DDF7C;
extern M2C_UNK D_002DEB30;
extern s32 D_002E1E40;
extern M2C_UNK D_002E65E0;
extern M2C_UNK D_003D4A60;
extern M2C_UNK D_003DD040;
extern M2C_UNK D_003DD490;
extern s32 D_003E0FE0;

/** @sly2-annot addr=0x224350
 * Purpose: base 0x1F4FB0 then out->0x98 = 0.25
 * Info: class CID100 slot55; TU tu_223F70
 * Notes: descriptive only: Cid100 slot (sets 0x98 = 0.25 on out struct) | no usable identifier; purpose only
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r8_21BB88+verdicts_A2
 * @end-sly2-annot */
f32 func_00224350(s32 arg0, s32 arg1) {
    func_001F4FB0();
    M2C_FIELD(arg1, f32 *, 0x98) = 0.25f;
    return 0.25f;
}
