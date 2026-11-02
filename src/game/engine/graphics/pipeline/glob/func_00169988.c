#include "m2c_prelude.h"
/** @sly2-annot addr=0x169988
 * Purpose: qsort comparator over 4 int keys (>>6 granularity on two)
 * Info: proposed name NCmpGlobKeys (T2); TU glob
 * Notes: no usable identifier; purpose only
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r3_1539A8+rendering
 * See: docs/reference/functions/rendering.md#glob-0x167310-0x16f148
 * @end-sly2-annot */
s32 func_00169988(void *arg0, void *arg1) {
    s32 var_v0;

    var_v0 = (s32) (M2C_FIELD(arg0, s32 *, 4) - M2C_FIELD(arg1, s32 *, 4)) >> 6;
    if (var_v0 == 0) {
        var_v0 = M2C_FIELD(arg0, s32 *, 8) - M2C_FIELD(arg1, s32 *, 8);
        if (var_v0 == 0) {
            var_v0 = (s32) (M2C_FIELD(arg0, s32 *, 0xC) - M2C_FIELD(arg1, s32 *, 0xC)) >> 6;
            if (var_v0 == 0) {
                return M2C_FIELD(arg0, s32 *, 0x10) - M2C_FIELD(arg1, s32 *, 0x10);
            }
        }
    }
    return var_v0;
}
