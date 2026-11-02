#include "m2c_prelude.h"
M2C_UNK func_001291A8(M2C_UNK *, M2C_UNK *, s32, s32); /* extern */
M2C_UNK func_00129EC8(M2C_UNK *);                   /* extern */
s32 func_001AE318(s32 *);                           /* extern */
extern f32 D_002DDED0;
extern M2C_UNK D_002DE578;
extern M2C_UNK D_002DEB30;
extern M2C_UNK D_002E1E88;
extern M2C_UNK D_002E65E0;
extern M2C_UNK D_003D4A60;
extern M2C_UNK D_003DA0C0;
extern M2C_UNK D_003DD490;
extern M2C_UNK D_003E0EA8;
extern void *D_003E0F90;
extern s32 D_003E0F9C;
extern void *D_003E0FA0;

/** @sly2-annot addr=0x162F10
 * Purpose: If the job has a completion widget run the completion sequence (phase 1), else FinishJob +
 *   ContinueAfterJob
 * Info: proposed name CompleteJob (T1); TU game
 * Notes: names_v2 v1 proposed CollectTurretPrize__FP6TURRET3PCKP3ALO (0.040)
 * Confidence: mid (draft) [T1 0.50] - evidence: static naming pass; sources: r3b_rest
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
void CompleteJob(s32 arg0, s32 arg1) {
    if (M2C_FIELD(arg0, s32 *, 0xB8) != 0) {
        SetJobCompletionPhase(arg0, 1);
        return;
    }
    FinishJob(arg0, arg1);
    func_00162FD0(arg0);
}
