#include "m2c_prelude.h"
/* Warning: missing "jr $ra" in last block of func_0022B040 (initial). */

s32 func_001AE318(s32 *, M2C_UNK);                  /* extern */
extern M2C_UNK D_002A2D00;
extern s128 D_002A2DA0;
extern s128 D_002A2DB0;
extern s128 D_002A2DC0;
extern s128 D_002A2DD0;
extern M2C_UNK D_002BBB60;
extern f32 D_002C4470;
extern f32 D_002C4480;
extern f32 D_002C449C;
extern M2C_UNK D_002C44A0;
extern M2C_UNK D_002C44B0;
extern M2C_UNK D_002C44C0;
extern M2C_UNK D_002C44D0;
extern f32 D_002DDED0;
extern f32 D_002DDED4;
extern M2C_UNK D_002DDF40;
extern M2C_UNK D_002DEB30;
extern M2C_UNK D_003DD490;
extern M2C_UNK D_003E10D0;
extern M2C_UNK D_00443740;

/** @sly2-annot addr=0x228358
 * Purpose: EXIT state change (Sly 1 SetExitExits): state 4 -> busyk 1, IncrementSwHandsOff(g_sw,3),
 *   g_sw+0x3C84 = this; time stamp
 * Info: proposed name SetExitExits (T1); TU exit_warp
 * Notes: v3: superseded SetWarpState | CONFIRMED (fields; TriggerWarp resets every EXIT through it with 0, as
 *   Sly 1 SetExitExits(pexit, EXITS_Blocked)); v2 SetWarpState
 * Confidence: mid (draft) [T1 0.70] - evidence: Sly 1 / Lilly reference; static naming pass; sources:
 *   r8_21BB88+class_review(master_names_v2_1)
 * See: docs/engine/world-sw.md#4-transitions-warps-exits-wipes-boot-queue,
 *   docs/reference/functions/world-sw.md#exit_warp-0x227fe8-0x228b48
 * @end-sly2-annot */
void SetExitExits(s32 arg0, s32 arg1) {
    if (arg1 != M2C_FIELD(arg0, s32 *, 0x13C)) {
        if (arg1 == 4) {
            SetLoBusyk(arg0, 1);
            IncrementSwHandsOff((s32) &D_003DD490, 3);
            M2C_FIELD(&D_003DD490, s32 *, 0x3C84) = arg0;
        }
        M2C_FIELD(arg0, s32 *, 0x13C) = arg1;
        M2C_FIELD(arg0, f32 *, 0x140) = (f32) D_002DDED0;
    }
}
