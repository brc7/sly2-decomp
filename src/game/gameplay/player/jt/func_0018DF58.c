#include "m2c_prelude.h"
/** @sly2-annot addr=0x18DF58
 * Purpose: Store vector at x1FA0
 * Info: proposed name SetJtX1FA0 (T2); TU jt
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/player.md#jt-0x188488-0x18e540
 * @end-sly2-annot */
void func_0018DF58(void *arg0, s128 arg1) {
    M2C_FIELD(arg0, s128 *, 0x1FA0) = arg1;
}
