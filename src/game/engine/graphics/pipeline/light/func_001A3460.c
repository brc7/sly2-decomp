#include "m2c_prelude.h"
M2C_UNK func_001A3968(void *);                      /* extern */
M2C_UNK func_001A5E80();                            /* extern */

/** @sly2-annot addr=0x1A3460
 * Purpose: LIGHT vt17: LO vt17, then if parent's x0A4 differs from own xE4 rebuild
 * Info: proposed name LIGHT_vt17 (T2); class LIGHT slot17; TU light
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/rendering.md#light-0x1a3260-0x1a4e08
 * @end-sly2-annot */
void func_001A3460(void *arg0) {
    s32 var_v1;
    void *temp_v0;

    func_001A5E80();
    temp_v0 = M2C_FIELD(arg0, void **, 0x50);
    var_v1 = 0;
    if (temp_v0 != NULL) {
        var_v1 = M2C_FIELD(temp_v0, s32 *, 0xA4);
    }
    if (M2C_FIELD(arg0, s32 *, 0xE4) < var_v1) {
        M2C_FIELD(arg0, s32 *, 0xE4) = var_v1;
        func_001A3968(arg0);
    }
}
