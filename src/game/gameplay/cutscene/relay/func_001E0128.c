#include "m2c_prelude.h"
/** @sly2-annot addr=0x1E0128
 * Purpose: If the signal-waiting JSG (+0x68 pjsgSignal) is on a Signal command (kind 9) return its +0x18
 *   (ploContext), else 0
 * Info: proposed name GetRelaySignalContext (T2); TU relay
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
s32 func_001E0128(void *arg0) {
    void *temp_a1;

    temp_a1 = M2C_FIELD(arg0, void **, 0x68);
    if ((temp_a1 == NULL) || (*M2C_FIELD(temp_a1, s32 **, 0x30) != 9)) {
        return 0;
    }
    return M2C_FIELD(temp_a1, s32 *, 0x18);
}
