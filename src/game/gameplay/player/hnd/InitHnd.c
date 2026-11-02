#include "m2c_prelude.h"
M2C_UNK InitGrabspot(s32);                         /* extern */
M2C_UNK InitTarget();                            /* extern */

/** @sly2-annot addr=0x184F50
 * Purpose: CID19 (HND) vt7: InitTarget then InitGrabspot on its GRABSPOT part
 * Info: proposed name InitHnd (T1); class HND slot7; TU hnd
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r4b_rest
 * See: docs/engine/core-library.md#6-source-files, docs/engine/player.md#10-source-files,
 *   docs/reference/functions/player.md#hnd-0x184f50-0x185130
 * @end-sly2-annot */
void InitHnd(void *arg0) {
    InitTarget();
    InitGrabspot(arg0 + 0x100);
    M2C_FIELD(arg0, s32 *, 0xE8) = 1;
    M2C_FIELD(arg0, s32 *, 0x12C) = 8;
    M2C_FIELD(arg0, s32 *, 0x130) = 9;
}
