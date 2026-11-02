#include "m2c_prelude.h"
f32 *func_0012F938();                               /* extern */

/** @sly2-annot addr=0x12F990
 * Purpose: Time of a label: PeaFindAsegLabel(...)->t
 * Info: proposed name TFindAsegLabel (T1); TU aseg
 * Notes: names_v2 v1 proposed CloneAseg__FP4ASEGP2LO (0.167)
 * Confidence: mid (draft) [T1 0.70] - evidence: static naming pass; sources: r1_116918
 * See: docs/reference/functions/animation.md#aseg-0x12f248-0x1308a8
 * @end-sly2-annot */
f32 func_0012F990(void) {
    f32 *temp_v0;
    f32 var_f0;

    temp_v0 = func_0012F938();
    var_f0 = 0.0f;
    if (temp_v0 != NULL) {
        var_f0 = *temp_v0;
    }
    return var_f0;
}
