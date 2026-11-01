#include "m2c_prelude.h"
M2C_UNK func_0018D698(void *, void *, void *);      /* extern */
M2C_UNK func_001CB4B8(s32, void *);                 /* extern */
s32 func_00224610(void *);                          /* extern */

/** @sly2-annot addr=0x136298
 * Purpose: Release object at +0x186C back to its manager
 * Info: proposed name ReleaseBentleyHeldObject (T2); TU bentley
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#bentley-0x134648-0x1378d8
 * @end-sly2-annot */
void func_00136298(void *arg0) {
    if (func_00224610(M2C_FIELD(arg0, void **, 0x186C)) != 0) {
        M2C_FIELD(M2C_FIELD(arg0, void **, 0x186C), void **, 0x16C) = arg0;
        func_0018D698(arg0, M2C_FIELD(arg0, void **, 0x16B4), M2C_FIELD(arg0, void **, 0x186C));
    }
    func_001CB4B8(M2C_FIELD(M2C_FIELD(arg0, void **, 0x16B4), s32 *, 4), M2C_FIELD(arg0, void **, 0x186C));
    M2C_FIELD(arg0, void **, 0x186C) = NULL;
    M2C_FIELD(arg0, void **, 0x16B4) = NULL;
}
