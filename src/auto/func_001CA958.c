#include "m2c_prelude.h"
/** @sly2-annot addr=0x1CA958
 * Purpose: inverse of IcopyFromPspare
 * Info: proposed name PspareFromIcopy (T1); TU proxy
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/object-model.md#proxy-0x1c9940-0x1cb908
 * @end-sly2-annot */
s32 func_001CA958(void *arg0, s32 arg1) {
    void *temp_v1;

    temp_v1 = M2C_FIELD(arg0, void **, 0xE0);
    return M2C_FIELD(temp_v1, s32 *, 0x68) + ((arg1 - M2C_FIELD(temp_v1, s32 *, 0xC)) * 0x50);
}
