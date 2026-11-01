#include "m2c_prelude.h"
/** @sly2-annot addr=0x15C310
 * Purpose: EXPLG: returns child count (+0xE0) and child array (+0xE4) via out-param
 * Info: proposed name CploExplgChildren (T1); class EXPLG slot28; TU emitter
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r3_1539A8
 * See: docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
s32 CploExplgChildren(void *arg0, s32 *arg1) {
    *arg1 = M2C_FIELD(arg0, s32 *, 0xE4);
    return M2C_FIELD(arg0, s32 *, 0xE0);
}
