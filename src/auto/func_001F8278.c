#include "m2c_prelude.h"
s32 func_001AE4F8(M2C_UNK);                         /* extern */
s32 func_001F8240();                                /* extern */
M2C_UNK func_0020A5F8(s32, s32);                    /* extern */

/** @sly2-annot addr=0x1F8278
 * Purpose: Find, else new 0xAC object (0x20A5F8)
 * Info: proposed name EnsureGlobalById (T2); TU so
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F8278(s32 arg0) {
    if (func_001F8240() == 0) {
        func_0020A5F8(func_001AE4F8(0xAC), arg0);
    }
}
