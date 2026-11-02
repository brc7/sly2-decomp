#include "m2c_prelude.h"
/** @sly2-annot addr=0x1C4728
 * Purpose: (grfbtn(+0x30) & grf) != 0 (held test)
 * Info: proposed name POCTL_FButtonDown (T1); class POCTL; TU po
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/player.md#po-0x1c3d70-0x1c4e50
 * @end-sly2-annot */
s32 POCTL_FButtonDown(void *arg0, s32 arg1) {
    return (M2C_FIELD(arg0, s32 *, 0x30) & arg1) != 0;
}
