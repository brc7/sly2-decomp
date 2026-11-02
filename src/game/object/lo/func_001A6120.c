#include "m2c_prelude.h"
extern s128 D_002A2DD0;

/** @sly2-annot addr=0x1A6120
 * Purpose: LO vt33: return zero vector D_002A2DD0
 * Info: proposed name GetLoVelocity (T2); class LO slot33; TU lo
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r4_17F090
 * See: docs/engine/object-model.md#2-the-lo-virtual-interface,
 *   docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518
 * @end-sly2-annot */
s128 func_001A6120(void) {
    return D_002A2DD0;
}
