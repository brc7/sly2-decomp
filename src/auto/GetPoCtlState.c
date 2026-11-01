#include "m2c_prelude.h"
/** @sly2-annot addr=0x1C4080
 * Purpose: Returns pctl->nState (+0x80) or -1 if PO has no POCTL
 * Info: proposed name GetPoCtlState (T1); TU po
 * Notes: names_v2 C proposed AdjustRobDifficulty__FP3ROBf (0.638)
 * Confidence: mid (draft) [T1 0.60] - evidence: static naming pass; sources: r5_1AE130
 * See: docs/reference/functions/player.md#po-0x1c3d70-0x1c4e50
 * @end-sly2-annot */
s32 GetPoCtlState(void *arg0) {
    s32 var_v0;
    void *temp_v1;

    temp_v1 = M2C_FIELD(arg0, void **, 0x150);
    var_v0 = -1;
    if (temp_v1 != NULL) {
        var_v0 = M2C_FIELD(temp_v1, s32 *, 0x80);
    }
    return var_v0;
}
