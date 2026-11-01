#include "m2c_prelude.h"
extern M2C_UNK D_002A75E0;

/** @sly2-annot addr=0x1B9B60
 * Purpose: MURRAY vt103: returns the string \murray\" (D_002A75E0)"
 * Info: proposed name MURRAY_PchName (T1); class MURRAY slot103; TU murray
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
M2C_UNK *func_001B9B60(void) {
    return &D_002A75E0;
}
