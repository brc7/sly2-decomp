#include "m2c_prelude.h"
/** @sly2-annot addr=0x1EF0C0
 * Purpose: BHV vt14 base: on the record passed (JT/STEPPLAYER +0x150 input/joy record): +0x1C=0,
 *   +0x18=pstep->+0x1A8
 * Info: proposed name UpdateBhvInput (T2); class BHV slot14; TU bhvstk
 * Notes: agent qualifier:
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/guards-ai.md#bhvstk-0x1eec60-0x1ef850,
 *   docs/reference/functions/player.md#tu_144b28-0x144b28-0x1471c0
 * @end-sly2-annot */
void func_001EF0C0(void *arg0, void *arg1) {
    M2C_FIELD(arg1, s32 *, 0x1C) = 0;
    M2C_FIELD(arg1, f32 *, 0x18) = (f32) M2C_FIELD(M2C_FIELD(arg0, void **, 0x10), f32 *, 0x1A8);
}
