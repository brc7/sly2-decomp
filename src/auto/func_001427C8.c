#include "m2c_prelude.h"
M2C_UNK func_00142860(void *);                      /* extern */
extern f32 D_002A4D60;

/** @sly2-annot addr=0x1427C8
 * Purpose: Set ploLean (+0x10F8) / nLeanKind (+0x10FC); on a new target with idle timer armed set +0x1148=1
 *   and reset +0x1100
 * Info: proposed name SetStepplayerLeanTarget (T1); TU stepplayer
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void func_001427C8(void *arg0, s32 arg1, s32 arg2) {
    if ((arg2 != M2C_FIELD(arg0, s32 *, 0x10F8)) || (arg1 != M2C_FIELD(arg0, s32 *, 0x10FC))) {
        M2C_FIELD(arg0, s32 *, 0x10FC) = arg1;
        M2C_FIELD(arg0, s32 *, 0x10F8) = arg2;
        if (arg2 != 0) {
            if (M2C_FIELD(arg0, f32 *, 0x114C) != D_002A4D60) {
                M2C_FIELD(arg0, s32 *, 0x1148) = 1;
                func_00142860(arg0);
            }
        }
    }
}
