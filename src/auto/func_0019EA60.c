#include "m2c_prelude.h"
M2C_UNK func_0019FD30(s32, void *, void *, f32);    /* extern */

/** @sly2-annot addr=0x19EA60
 * Purpose: If the pipe allows, record it in the STEPPLAYER+0x9E0 place table (0x19FD30)
 * Info: proposed name RecordJtPipePlace (T2); TU pipe_jt
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#pipe_jt-0x19de10-0x19f0a8
 * @end-sly2-annot */
void func_0019EA60(void *arg0) {
    void *temp_a2;

    temp_a2 = M2C_FIELD(arg0, void **, 0xC);
    if (M2C_FIELD(temp_a2, s32 *, 0x19C) == 0) {
        func_0019FD30(M2C_FIELD(arg0, s32 *, 0) + 0x9E0, (temp_a2 == NULL) ? NULL : (temp_a2 + 0x190), temp_a2, M2C_FIELD(arg0, f32 *, 0x10));
    }
}
