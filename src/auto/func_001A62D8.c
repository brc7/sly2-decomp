#include "m2c_prelude.h"
extern M2C_UNK D_002A2D30;

/** @sly2-annot addr=0x1A62D8
 * Purpose: LO vt31 default: return static identity D_002A2D30
 * Info: proposed name LO_PmatWorld (T1); class LO slot31; TU lo
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r4_17F090
 * See: docs/engine/object-model.md#2-the-lo-virtual-interface,
 *   docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518
 * @end-sly2-annot */
M2C_UNK *func_001A62D8(void) {
    return &D_002A2D30;
}
