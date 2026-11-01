#include "m2c_prelude.h"
M2C_UNK func_00231368();                            /* extern */

/** @sly2-annot addr=0x231440
 * Purpose: UpdateXfmMatWorld then return &matWorld (+0xA0)
 * Info: proposed name XFM_PmatWorld (T1); class XFM slot31; TU xform
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/object-model.md#xform-0x231100-0x2319e8
 * @end-sly2-annot */
s32 func_00231440(s32 arg0) {
    func_00231368();
    return arg0 + 0xA0;
}
