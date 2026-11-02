#include "m2c_prelude.h"
/** @sly2-annot addr=0x1841F0
 * Purpose: mask(+0xFC) & player's class bit (+0x1000) != 0
 * Info: proposed name FGrabspotAllowsPlayer (T2); TU grabspot
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#grabspot-0x182c70-0x1848a8
 * @end-sly2-annot */
s32 func_001841F0(void *arg0, void *arg1) {
    return (M2C_FIELD(arg0, s32 *, 0xFC) & M2C_FIELD(arg1, s32 *, 0x1000)) != 0;
}
