#include "m2c_prelude.h"
M2C_UNK InitBhv();                            /* extern */
M2C_UNK AddStepguardSgf(s32, M2C_UNK);                /* extern */

/** @sly2-annot addr=0x1BA560
 * Purpose: BHV174 s7: base, state -1, AddStepguardSgf(0x25)
 * Info: proposed name InitBhv174 (T2); class BHV174 slot7; TU tu_1B9F80
 * Confidence: low (draft) [T2 0.50] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/guards-ai.md#tu_1b9f80-0x1b9f80-0x1bab50
 * @end-sly2-annot */
void func_001BA560(void *arg0) {
    InitBhv();
    M2C_FIELD(arg0, s32 *, 0x30) = -1;
    AddStepguardSgf(M2C_FIELD(arg0, s32 *, 0x10), 0x25);
}
