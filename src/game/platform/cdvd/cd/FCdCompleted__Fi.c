#include "m2c_prelude.h"
s32 sceCdSync(s32);                                 /* extern */

/** @sly2-annot addr=0x14BD30
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#cd-0x14b7f8-0x14bea8
 * @end-sly2-annot */
s32 FCdCompleted__Fi(s32 arg0) {
    return sceCdSync(arg0 == 0) == 0;
}
