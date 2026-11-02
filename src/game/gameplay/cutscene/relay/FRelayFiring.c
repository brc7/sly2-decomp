#include "m2c_prelude.h"
/** @sly2-annot addr=0x1DFFD8
 * Purpose: return fFiring (+0x74)
 * Info: proposed name FRelayFiring (T1); TU relay
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
s32 FRelayFiring(void *arg0) {
    return M2C_FIELD(arg0, s32 *, 0x74);
}
