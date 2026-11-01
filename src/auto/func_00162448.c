#include "m2c_prelude.h"
M2C_UNK func_0011A050(s32, M2C_UNK);                /* extern */
M2C_UNK func_001A5270();                            /* extern */

/** @sly2-annot addr=0x162448
 * Purpose: JOB (cid 0x84) Init: LO Init, difficulty-ease thresholds 480/720 s, ids -1, InitDl(+0x80, link
 *   0x84) for its tasks
 * Info: proposed name InitJob (T1); class JOB slot7; TU game
 * Notes: superseded r3_1539A8 proposal: InitCid84 (0.75)
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r3b_rest+verdicts_tu_r3r5
 * See: docs/engine/game-progression.md#8-key-functions,
 *   docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
void func_00162448(void *arg0) {
    func_001A5270();
    M2C_FIELD(arg0, f32 *, 0x9C) = 480.0f;
    M2C_FIELD(arg0, f32 *, 0xA0) = 720.0f;
    M2C_FIELD(arg0, s32 *, 0x6C) = -1;
    M2C_FIELD(arg0, s32 *, 0x70) = -1;
    M2C_FIELD(arg0, s32 *, 0xAC) = -1;
    M2C_FIELD(arg0, s32 *, 0xB0) = -1;
    M2C_FIELD(arg0, s32 *, 0xB4) = -1;
    func_0011A050(arg0 + 0x80, 0x84);
}
