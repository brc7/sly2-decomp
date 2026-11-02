#include "m2c_prelude.h"
/** @sly2-annot addr=0x1BF970
 * Purpose: Index of a graph vertex: (pcgv - pcg->acgv) >> 5
 * Info: proposed name IcgvFromPcgv (T1); TU path
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r5b_rest+verdicts_A2
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#path-0x1bf110-0x1c2920
 * @end-sly2-annot */
s32 IcgvFromPcgv__FP2CGP3CGV(void *arg0, s32 arg1) {
    return (s32) (arg1 - M2C_FIELD(arg0, s32 *, 4)) >> 5;
}
