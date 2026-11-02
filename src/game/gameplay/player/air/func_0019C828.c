#include "m2c_prelude.h"
extern s128 D_002A2DD0;
extern f32 D_002A65B8;
extern f32 D_002A65BC;
extern f32 D_002A6670;
extern f32 D_002A6674;
extern f32 D_002A66A0;
extern f32 D_002BF8E0;
extern f32 D_002BF8E4;
extern f32 D_002BF8E8;
extern f32 D_002BF8EC;
extern f32 D_002BF8F0;
extern f32 D_002BF8F4;
extern f32 D_002BF8F8;
extern f32 D_002BF8FC;
extern f32 D_002BF904;
extern f32 D_002BF908;
extern f32 D_002BF90C;
extern f32 D_002BF914;
extern f32 D_002BF918;
extern s32 D_002BF920;
extern M2C_UNK D_002BF928;
extern f32 D_002BF938;
extern f32 D_002BF940;
extern f32 D_002BF944;
extern M2C_UNK D_002BF9D0;
extern M2C_UNK D_002BF9E0;
extern f32 D_002BF9F0;
extern M2C_UNK D_002BFA30;
extern s32 D_002DD5BC;
extern f32 D_002DDED0;
extern f32 D_002DDED4;
extern M2C_UNK D_002DDF40;
extern M2C_UNK D_002DE2E0;
extern M2C_UNK D_002DEB30;
extern s32 D_002DFBC0;
extern s32 D_002E1E40;
extern void *D_002F7900;
extern M2C_UNK D_003DD040;
extern M2C_UNK D_003DD490;
extern void *D_003E13B4;
extern void *D_003E13C0;
extern void *D_003E13CC;
extern void *D_003E13D8;
extern void *D_003E13F0;
extern void *D_003E13FC;
extern void *D_003E1408;
extern s32 D_003E142C;
extern M2C_UNK D_004435F0;

/** @sly2-annot addr=0x19C828
 * Purpose: Kind 0x11 (wall run) -> SetJtAirJump(3); else SetJtAirState(4)
 * Info: proposed name LandJtAir (T2); TU air
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#air-0x195950-0x19cbf8
 * @end-sly2-annot */
void func_0019C828(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    if (M2C_FIELD(arg0, s32 *, 0x1C) == 0x11) {
        func_00195A00(arg0, 3, 0, 0);
        return;
    }
    func_00195FB8(arg0, 4, 0);
}
