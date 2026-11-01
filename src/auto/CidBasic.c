#include "m2c_prelude.h"
/** @sly2-annot addr=0x132EA0
 * Purpose: Returns Pclassinfo()->cid (vt6)
 * Info: proposed name CidBasic (T1); TU basic
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/object-model.md#basic-0x132b00-0x1336f0
 * @end-sly2-annot */
s32 CidBasic(void *arg0) {
    s16 temp_a1;
    void *temp_v1;

    temp_v1 = M2C_FIELD(arg0, void **, 0xC);
    temp_a1 = M2C_FIELD(temp_v1, s16 *, 0x30);
    return M2C_FIELD(M2C_FIELD(temp_v1, void *(**)(void *, s16), 0x34)(arg0 + temp_a1, temp_a1), s32 *, 4);
}
