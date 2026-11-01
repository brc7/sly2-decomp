#include "m2c_prelude.h"
/** @sly2-annot addr=0x1CA930
 * Purpose: (pspare - aspare)/0x50 + first spare copy index
 * Info: proposed name IcopyFromPspare (T1); TU proxy
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r5_1AE130
 * See: docs/engine/object-model.md#9-key-functions,
 *   docs/reference/functions/object-model.md#proxy-0x1c9940-0x1cb908
 * @end-sly2-annot */
s32 func_001CA930(void *arg0, s32 arg1) {
    void *temp_a2;

    temp_a2 = M2C_FIELD(arg0, void **, 0xE0);
    return ((s32) ((arg1 - M2C_FIELD(temp_a2, s32 *, 0x68)) * 0xCCCCCCCD) >> 4) + M2C_FIELD(temp_a2, s32 *, 0xC);
}
