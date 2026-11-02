#include "m2c_prelude.h"
s32 func_001BBE18();                                /* extern */

/** @sly2-annot addr=0x1BBE68
 * Purpose: Current stage record or NULL
 * Info: proposed name PstageCurCid98 (T2); class CID98; TU tu_1BAB50
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/gameplay-objects.md#tu_1bab50-0x1bab50-0x1bc050
 * @end-sly2-annot */
s32 func_001BBE68(void *arg0) {
    if (func_001BBE18() == 0) {
        return 0;
    }
    return M2C_FIELD(arg0, s32 *, 0x164) + (M2C_FIELD(arg0, s32 *, 0x15C) * 0x10);
}
