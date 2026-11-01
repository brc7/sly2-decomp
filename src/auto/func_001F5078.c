#include "m2c_prelude.h"
M2C_UNK CopyAb__FPvT0Ui(s32, s32, s32);             /* extern */
void **func_001AE318(s32 *);                        /* extern */
extern s128 D_002A2DD0;
extern M2C_UNK D_002C3C20;
extern M2C_UNK D_002C44F0;
extern void *D_002DD2FC;
extern f32 D_002DDED0;
extern f32 D_002DDED4;
extern M2C_UNK D_003DD490;
extern void *D_003E1218;
extern void *D_003E126C;
extern M2C_UNK D_003E15A8;
extern s32 *D_003E15C0;

/** @sly2-annot addr=0x1F5078
 * Purpose: If the SO is in the broadphase (+0xEC): SetSoGrf(4) (vt47, the awake flag also set by
 *   SlotSetRole/WakeXfSlot) and, if +0xE8, un-settle its position and rotation XF slots (SlotUnsettle
 *   0x230598)
 * Info: proposed name WakeSo (T2); TU so
 * Notes: v3: superseded InvalidateSoPhys | top50_round (0.45): Body: vt[47](4) on the SO, then 0x230598 on
 *   +0x58 (position XF) and +0x5C (rotation XF). 0x230598 maps every at-rest role to its moving counterpart
 *   (decoded jump table D_002AA000: 3->1, 6/10->7, 12->11, 19->18), the inve
 * Confidence: low (draft) [T2 0.45] - evidence: semantic review; sources: r6_1D5150+top50_round
 * See: docs/engine/physics-collision.md#7-key-functions, docs/open-questions.md#physics--navigation,
 *   docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_001F5078(s32 arg0) {
    void *temp_v0;

    if (M2C_FIELD(arg0, s32 *, 0xEC) != 0) {
        temp_v0 = M2C_FIELD(arg0, void **, 0xC);
        M2C_FIELD(temp_v0, void (**)(s32, M2C_UNK), 0x17C)(arg0 + M2C_FIELD(temp_v0, s16 *, 0x178), 4);
        if (M2C_FIELD(arg0, s32 *, 0xE8) != 0) {
            func_00230598(arg0, M2C_FIELD(arg0, s32 *, 0x58));
            func_00230598(arg0, M2C_FIELD(arg0, s32 *, 0x5C));
        }
    }
}
