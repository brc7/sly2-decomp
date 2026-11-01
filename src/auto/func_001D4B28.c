#include "m2c_prelude.h"
extern f32 D_002DDED0;

/** @sly2-annot addr=0x1D4B28
 * Purpose: Set AI tactic x188 with timestamp x18C
 * Info: proposed name SetRobottankTactic (T2); TU tu_1D1370
 * Notes: class identity from whole_game WE names/object strings: SetRcCarTactic -> SetRobottankTactic
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r5b_rest+whole_game(class identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_1d1370-0x1d1370-0x1d5150
 * @end-sly2-annot */
void func_001D4B28(void *arg0, s32 arg1) {
    if (arg1 != M2C_FIELD(arg0, s32 *, 0x188)) {
        M2C_FIELD(arg0, s32 *, 0x188) = arg1;
        M2C_FIELD(arg0, f32 *, 0x18C) = (f32) D_002DDED0;
    }
}
