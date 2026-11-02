#include "m2c_prelude.h"
void *func_001CB410();                              /* extern */

/** @sly2-annot addr=0x1CB488
 * Purpose: spare(plo)->fLocked=1
 * Info: proposed name LockProxyPlo (T1); TU proxy
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/object-model.md#proxy-0x1c9940-0x1cb908
 * @end-sly2-annot */
void LockProxyPlo(void) {
    void *temp_v0;

    temp_v0 = func_001CB410();
    if (temp_v0 != NULL) {
        M2C_FIELD(temp_v0, s32 *, 8) = 1;
    }
}
