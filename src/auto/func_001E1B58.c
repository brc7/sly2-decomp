#include "m2c_prelude.h"
/** @sly2-annot addr=0x1E1B58
 * Purpose: cref++ (+0x10)
 * Info: proposed name AddJsgRef (T1); class JSG; TU relay
 * Notes: agent qualifier: (IncrefJsg)
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r6b_rest
 * See: docs/engine/cutscenes.md#5-key-functions,
 *   docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void func_001E1B58(void *arg0) {
    M2C_FIELD(arg0, s32 *, 0x10) = (s32) (M2C_FIELD(arg0, s32 *, 0x10) + 1);
}
