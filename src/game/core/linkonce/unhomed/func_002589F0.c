#include "m2c_prelude.h"
/** @sly2-annot addr=0x2589F0
 * Purpose: return pemitb->field4 (EMITTER copy) [game linkonce (inline/template)]
 * Info: proposed name GetEmitterEmitbField4 (T2); TU game_linkonce
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
s32 func_002589F0(void *arg0) {
    return M2C_FIELD(M2C_FIELD(arg0, void **, 0xE4), s32 *, 4);
}
