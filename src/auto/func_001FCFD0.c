#include "m2c_prelude.h"
void *func_001FCFA0();                              /* extern */

/** @sly2-annot addr=0x1FCFD0
 * Purpose: Float field +0x50 of a stream record found by id, else 0
 * Info: proposed name GDialogStreamParam (T2); TU sound
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
f32 func_001FCFD0(void) {
    f32 var_f0;
    void *temp_v0;

    temp_v0 = func_001FCFA0();
    var_f0 = 0.0f;
    if (temp_v0 != NULL) {
        var_f0 = M2C_FIELD(temp_v0, f32 *, 0x50);
    }
    return var_f0;
}
