#include "m2c_prelude.h"
/** @sly2-annot addr=0x184028
 * Purpose: Splice property op: mask(+0xFC)
 * Info: proposed name AddGrabspotPlayerMask (T2); TU grabspot
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#grabspot-0x182c70-0x1848a8
 * @end-sly2-annot */
void func_00184028(void *arg0, void *arg1) {
    M2C_FIELD(arg0, s32 *, 0xFC) = (s32) (M2C_FIELD(arg0, s32 *, 0xFC) | M2C_FIELD(arg1, s32 *, 0x1000));
}
