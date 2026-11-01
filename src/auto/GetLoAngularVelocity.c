#include "m2c_prelude.h"
extern s128 D_002A2DD0;

/** @sly2-annot addr=0x1A6138
 * Purpose: LO vt34: return zero vector
 * Info: proposed name GetLoAngularVelocity (T1); class LO slot34; TU lo
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r4_17F090+verdicts_tu_r4r6
 * See: docs/engine/object-model.md#9-key-functions,
 *   docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518
 * @end-sly2-annot */
s128 GetLoAngularVelocity(void) {
    return D_002A2DD0;
}
