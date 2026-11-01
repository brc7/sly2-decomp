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

/** @sly2-annot addr=0x19CC50
 * Purpose: Request the look/recon mode of kind n: if running and switching to kind 1, end (2) and reset (-1)
 *   first; store kind (+0x20) and enter state 0
 * Info: proposed name StartJtMode (T1); TU mode
 * Notes: runtime4: started by 0x400 + FCanUseGadget(3) in Murray switch = JTMODE look mode | runtime5a
 *   #9-#11: JTMODE kind 0 = binocucom for Sly/Murray/Bentley (shared component +0xF00)
 * Confidence: mid (draft) [T1 0.60] - evidence: runtime (save-state RAM); sources: r4b_rest+runtime
 * See: docs/reference/functions/player.md#mode-0x19cbf8-0x19de10
 * @end-sly2-annot */
void func_0019CC50(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    if (M2C_FIELD(arg0, s32 *, 4) != -1) {
        if ((M2C_FIELD(arg0, s32 *, 0x20) != 1) && (arg1 == 1)) {
            func_0019CD38(arg0, 2);
            func_0019CD38(arg0, -1);
            goto block_4;
        }
    } else {
block_4:
        M2C_FIELD(arg0, s32 *, 0x20) = arg1;
        M2C_FIELD(arg0, s32 *, 0x40) = arg2;
        M2C_FIELD(arg0, s32 *, 0x44) = arg3;
        func_0019CD38(arg0, 0);
    }
}
