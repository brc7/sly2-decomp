#include "m2c_prelude.h"
extern M2C_UNK D_002A2D00;
extern s128 D_002A2DA0;
extern s128 D_002A2DC0;
extern s128 D_002A2DD0;
extern M2C_UNK D_002BBB60;
extern f32 D_002BF3A0;
extern M2C_UNK D_002BF3A8;
extern M2C_UNK D_002BF3B0;
extern f32 D_002BF3B8;
extern f32 D_002BF3BC;
extern M2C_UNK D_002BF3F8;
extern M2C_UNK D_002BF408;
extern f32 D_002BF414;
extern M2C_UNK D_002BF418;
extern f32 D_002BF4B8;
extern M2C_UNK D_002BF4C0;
extern M2C_UNK D_002BF4D0;
extern M2C_UNK D_002BF4E0;
extern M2C_UNK D_002BF4F0;
extern M2C_UNK D_002BF500;
extern f32 D_002BF510;
extern M2C_UNK D_002BF520;
extern M2C_UNK D_002BF530;
extern f32 D_002BF544;
extern M2C_UNK D_002BF548;
extern f32 D_002BF554;
extern f32 D_002BF558;
extern f32 D_002BF55C;
extern f32 D_002BF560;
extern f32 D_002BF564;
extern f32 D_002BF568;
extern f32 D_002BF56C;
extern M2C_UNK D_002BF570;
extern M2C_UNK D_002BF580;
extern f32 D_002BF58C;
extern f32 D_002BF590;
extern f32 D_002BF594;
extern M2C_UNK D_002BF598;
extern f32 D_002DDED0;
extern f32 D_002DDED4;
extern M2C_UNK D_002DE2E0;
extern M2C_UNK D_003DD490;
extern void *D_003E13B4;
extern s32 D_003E1BEC;

/** @sly2-annot addr=0x18DE18
 * Purpose: (pjt,obj): obj -> x9C0=1 and set grab sub-state kind 6 on x4C0 (0x1928F8); null -> release if kind
 *   6
 * Info: proposed name SetJtGrabObject (T2); class JT; TU jt
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/player.md#jt-0x188488-0x18e540
 * @end-sly2-annot */
void func_0018DE18(s32 arg0, s32 arg1) {
    void *temp_v0;

    if (arg1 != 0) {
        M2C_FIELD(arg0, s32 *, 0x9C0) = 1;
        SetJtGrabState(arg0 + 0x4C0, 6, arg1, 0);
    } else if (M2C_FIELD(arg0, s32 *, 0x4C8) == 6) {
        temp_v0 = M2C_FIELD(arg0, void **, 0xC);
        M2C_FIELD(temp_v0, void (**)(s32, M2C_UNK), 0x2C4)(arg0 + M2C_FIELD(temp_v0, s16 *, 0x2C0), 0);
    }
}
