#include "m2c_prelude.h"
s32 func_00132B10(void *, M2C_UNK);                 /* extern */

/** @sly2-annot addr=0x177D70
 * Purpose: Guard is cid 0x3C and +0x108C is set
 * Info: proposed name FBhvguardUseNeelaBhv (T2); TU guardbhv
 * Notes: class identity from whole_game WE names/object strings: FBhvguardUseCid60Bhv -> FBhvguardUseNeelaBhv
 *   | runtime2/4 confirmed the BHVGUARD role on live guards (S02-S41)
 * Confidence: low (draft) [T2 0.45] - evidence: runtime (save-state RAM); sources:
 *   r9_bhv_screen+whole_game(class identity)+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
s32 func_00177D70(void *arg0) {
    s32 var_v0;

    var_v0 = 0;
    if (func_00132B10(M2C_FIELD(arg0, void **, 0x10), 0x3C) != 0) {
        var_v0 = M2C_FIELD(M2C_FIELD(arg0, void **, 0x10), s32 *, 0x108C) != 0;
    }
    return var_v0;
}
