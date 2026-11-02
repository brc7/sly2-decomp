#include "m2c_prelude.h"
M2C_UNK func_001C1968(s32, void *);                 /* extern */

/** @sly2-annot addr=0x1BD1E0
 * Purpose: RemovePathzoneObstacle if registered
 * Info: proposed name UnregisterCid14Obstacle (T2); class CID14; TU tu_1BC050
 * Confidence: low (draft) [T2 0.50] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/physics-collision.md#tu_1bc050-0x1bc050-0x1be398
 * @end-sly2-annot */
void func_001BD1E0(void *arg0) {
    s32 temp_a0;

    temp_a0 = M2C_FIELD(arg0, s32 *, 0x1DC);
    if (temp_a0 != 0) {
        func_001C1968(temp_a0, arg0);
        M2C_FIELD(arg0, s32 *, 0x1DC) = 0;
    }
}
