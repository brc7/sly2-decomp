#include "m2c_prelude.h"
/** @sly2-annot addr=0x1327D0
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#bas-0x132570-0x132b00
 * @end-sly2-annot */
s32 FSpooling__18CBinaryAsyncStream(void *arg0) {
    return M2C_FIELD(arg0, s32 *, 0xC) != 0;
}
