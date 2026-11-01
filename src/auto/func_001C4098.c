#include "m2c_prelude.h"
s32 func_001C4080();                                /* extern */

/** @sly2-annot addr=0x1C4098
 * Purpose: True if GetPoCtlState != -1
 * Info: proposed name FPlayablePo (T2); TU po
 * Notes: names_v2 B proposed ReturnedRobRoh__FP3ROBP3ROH (0.856)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r5_1AE130
 * See: docs/reference/functions/player.md#po-0x1c3d70-0x1c4e50
 * @end-sly2-annot */
s32 func_001C4098(void) {
    return ~func_001C4080() != 0;
}
