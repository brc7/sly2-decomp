#include "m2c_prelude.h"
/** @sly2-annot addr=0x1E0158
 * Purpose: Setter counterpart of 0x1E0128
 * Info: proposed name SetRelaySignalContext (T2); TU relay
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void func_001E0158(void *arg0, s32 arg1) {
    void *temp_a2;

    temp_a2 = M2C_FIELD(arg0, void **, 0x68);
    if ((temp_a2 != NULL) && (*M2C_FIELD(temp_a2, s32 **, 0x30) == 9)) {
        M2C_FIELD(temp_a2, s32 *, 0x18) = arg1;
    }
}
