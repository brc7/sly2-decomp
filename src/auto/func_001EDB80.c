#include "m2c_prelude.h"
/** @sly2-annot addr=0x1EDB80
 * Purpose: SAA vt10: *poid == psaa->+0x14
 * Info: proposed name SAA_FMatches (T2); class SAA slot10; TU shdanim
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/rendering.md#shdanim-0x1ed7a8-0x1ee988
 * @end-sly2-annot */
s32 func_001EDB80(void *arg0, s32 *arg1) {
    return *arg1 == M2C_FIELD(arg0, s32 *, 0x14);
}
