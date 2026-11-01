#include "m2c_prelude.h"
extern void *D_002C49D4;

/** @sly2-annot addr=0x2342A0
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/sdk-libraries.md#sdk_libs-0x233000-0x254f00
 * @end-sly2-annot */
s32 rand(void) {
    s32 temp_v1;

    temp_v1 = (M2C_FIELD(D_002C49D4, s32 *, 0x58) * 0x41C64E6D) + 0x3039;
    M2C_FIELD(D_002C49D4, s32 *, 0x58) = temp_v1;
    return temp_v1 & 0x7FFFFFFF;
}
