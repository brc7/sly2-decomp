#include "m2c_prelude.h"
/** @sly2-annot addr=0x17A9C0
 * Purpose: state == -1
 * Info: proposed name FBhv176Done (T2); TU guardbhv
 * Notes: BHV class word normalised: FBhvthrownDone -> FBhv176Done
 * Confidence: low (draft) [T2 0.50] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
s32 func_0017A9C0(void *arg0) {
    return ~M2C_FIELD(arg0, s32 *, 0x30) == 0;
}
