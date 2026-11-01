#include "m2c_prelude.h"
void *func_001CB410();                              /* extern */

/** @sly2-annot addr=0x1CB4B8
 * Purpose: spare(plo)->fLocked=0
 * Info: proposed name UnlockProxyPlo (T1); TU proxy
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/object-model.md#proxy-0x1c9940-0x1cb908
 * @end-sly2-annot */
void func_001CB4B8(void) {
    void *temp_v0;

    temp_v0 = func_001CB410();
    if (temp_v0 != NULL) {
        M2C_FIELD(temp_v0, s32 *, 8) = 0;
    }
}
