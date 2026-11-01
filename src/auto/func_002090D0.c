#include "m2c_prelude.h"
M2C_UNK func_0020C030(s32);                         /* extern */
M2C_UNK func_0020CA20(s32);                         /* extern */

/** @sly2-annot addr=0x2090D0
 * Purpose: Free kind-2/3 payload objects (0x20C030/0x20CA20) and mark binding empty (-1)
 * Info: proposed name ReleaseAnimBinding (T1); TU stepguard
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7b_guard+verdicts_followups
 * See: docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_002090D0(void *arg0) {
    s32 temp_v1;

    temp_v1 = M2C_FIELD(arg0, s32 *, 0);
    if (temp_v1 != 2) {
        if (temp_v1 == 3) {
            func_0020CA20(M2C_FIELD(arg0, s32 *, 4));
            goto block_4;
        }
    } else {
        func_0020C030(M2C_FIELD(arg0, s32 *, 4));
block_4:
        M2C_FIELD(arg0, s32 *, 4) = 0;
    }
    M2C_FIELD(arg0, s32 *, 4) = -1;
    M2C_FIELD(arg0, s32 *, 0) = -1;
}
