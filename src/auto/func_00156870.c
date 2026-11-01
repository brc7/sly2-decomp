#include "m2c_prelude.h"
s32 func_0011A380(M2C_UNK *, s32);                  /* extern */
extern M2C_UNK D_003E148C;

/** @sly2-annot addr=0x156870
 * Purpose: True if the noise is no longer in the active list
 * Info: proposed name FNoiseRemoved (T2); TU noise
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/guards-ai.md#noise-0x1564e8-0x157200
 * @end-sly2-annot */
s32 func_00156870(s32 arg0) {
    return func_0011A380(&D_003E148C, arg0) == 0;
}
