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

/** @sly2-annot addr=0x225A68
 * Purpose: ClonePo then fix self pointer 0x1D0
 * Info: proposed name CloneTurret (T1); class TURRET slot41; TU turret
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r8_21BB88+verdicts_tu_r7r8
 * See: docs/engine/gameplay-objects.md#key-functions,
 *   docs/reference/functions/gameplay-objects.md#turret-0x2259d8-0x226550
 * @end-sly2-annot */
s32 func_00225A68(s32 arg0, s32 arg1) {
    s32 temp_v0;

    func_001C3DF8(arg0, arg1);
    temp_v0 = M2C_FIELD(arg0, s32 *, 0x1D0);
    if (temp_v0 == arg1) {
        M2C_FIELD(arg0, s32 *, 0x1D0) = arg0;
    }
    return temp_v0;
}
