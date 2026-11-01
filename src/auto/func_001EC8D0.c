#include "m2c_prelude.h"
s32 func_001EC880();                                /* extern */
extern s32 D_003E1590;

/** @sly2-annot addr=0x1EC8D0
 * Purpose: 0x1EC880(oid) or the list head
 * Info: proposed name PfontFindOrDefault (T2); TU shd
 * Notes: names_v2 C proposed ResetGsMemory__Fv (0.638)
 * Confidence: low (draft) [T2 0.30] - evidence: static naming pass; sources: r6b_rest
 * See: docs/reference/functions/rendering.md#shd-0x1ea568-0x1ed7a8
 * @end-sly2-annot */
s32 func_001EC8D0(void) {
    s32 var_v0;

    var_v0 = func_001EC880();
    if (var_v0 == 0) {
        var_v0 = D_003E1590;
    }
    return var_v0;
}
