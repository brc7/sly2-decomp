#include "m2c_prelude.h"
/** @sly2-annot addr=0x144038
 * Purpose: vt89 GetPlayerState in {5,6}
 * Info: proposed name FStepplayerInScriptState (T2); TU stepplayer
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
s32 func_00144038(void *arg0) {
    s16 temp_a1;
    void *temp_v1;

    temp_v1 = M2C_FIELD(arg0, void **, 0xC);
    temp_a1 = M2C_FIELD(temp_v1, s16 *, 0x2C8);
    return (u32) (M2C_FIELD(temp_v1, s32 (**)(void *, s16), 0x2CC)(arg0 + temp_a1, temp_a1) - 5) < 2U;
}
