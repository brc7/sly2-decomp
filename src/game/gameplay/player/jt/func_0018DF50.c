#include "m2c_prelude.h"
/** @sly2-annot addr=0x18DF50
 * Purpose: Store int at x1D30
 * Info: proposed name SetJtX1D30 (T2); TU jt
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/player.md#jt-0x188488-0x18e540
 * @end-sly2-annot */
void func_0018DF50(void *arg0, s32 arg1) {
    M2C_FIELD(arg0, s32 *, 0x1D30) = arg1;
}
