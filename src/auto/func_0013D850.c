#include "m2c_prelude.h"
M2C_UNK memcpy(s32, s32, s32);                      /* extern */

/** @sly2-annot addr=0x13D850
 * Purpose: Copy min(cb, remaining) bytes from memory source and advance
 * Info: proposed name CQueueInputMemory_CbRead (T1); class CQueueInputMemory; TU bq
 * Confidence: high [T1 0.90] - evidence: static analysis; sources: r2b_rest+verdicts_A1
 * See: docs/engine/core-library.md#7-key-functions,
 *   docs/reference/functions/core-library.md#bq-0x13d558-0x13da18
 * @end-sly2-annot */
s32 func_0013D850(void *arg0, s32 arg1, s32 arg2) {
    s32 temp_s0;
    s32 temp_s0_2;

    temp_s0_2 = M2C_FIELD(arg0, s32 *, 0x10);
    temp_s0 = (arg1 < temp_s0_2) ? arg1 : temp_s0_2;
    memcpy(arg2, M2C_FIELD(arg0, s32 *, 4) + M2C_FIELD(arg0, s32 *, 0xC), temp_s0);
    M2C_FIELD(arg0, s32 *, 0xC) = (s32) (M2C_FIELD(arg0, s32 *, 0xC) + temp_s0);
    M2C_FIELD(arg0, s32 *, 0x10) = (s32) (M2C_FIELD(arg0, s32 *, 0x10) - temp_s0);
    return temp_s0;
}
