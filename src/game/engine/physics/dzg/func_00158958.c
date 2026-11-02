#include "m2c_prelude.h"
/** @sly2-annot addr=0x158958
 * Purpose: Maps a DZ row pointer to its parallel 0x60-byte record (af + index*0x60)
 * Info: proposed name PxpFromDzgDz (T2); TU dzg
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/core-library.md#dzg-0x1585d8-0x159118
 * @end-sly2-annot */
s32 func_00158958(void *arg0, s32 arg1) {
    return M2C_FIELD(arg0, s32 *, 0x60) + (((s32) ((arg1 - M2C_FIELD(arg0, s32 *, 8)) * 0xCCCCCCCD) >> 3) * 0x60);
}
