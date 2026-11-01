#include "m2c_prelude.h"
/** @sly2-annot addr=0x171458
 * Purpose: True when no current command (+0x38 == 0)
 * Info: proposed name FBhv169ScriptDone (T2); TU tu_1709A8
 * Notes: v2: demoted to T2 (placeholder token in name: 169)
 * Confidence: low (draft) [T2 0.50] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/guards-ai.md#tu_1709a8-0x1709a8-0x171f58
 * @end-sly2-annot */
s32 func_00171458(void *arg0) {
    return M2C_FIELD(arg0, s32 *, 0x38) == 0;
}
