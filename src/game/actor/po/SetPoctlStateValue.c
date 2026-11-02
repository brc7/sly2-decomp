#include "m2c_prelude.h"
/** @sly2-annot addr=0x1C4640
 * Purpose: nState=n; when becoming active (1) set grfPressed=0xFFFF so already-held buttons don't count as
 *   presses
 * Info: proposed name SetPoctlStateValue (T1); class POCTL; TU po
 * Notes: names_v2 C proposed AdjustRocNewXp__FP3ROCP2XPi (0.200)
 * Confidence: mid (draft) [T1 0.70] - evidence: static naming pass; sources: r5_1AE130
 * See: docs/reference/functions/player.md#po-0x1c3d70-0x1c4e50
 * @end-sly2-annot */
void SetPoctlStateValue(void *arg0, s32 arg1) {
    M2C_FIELD(arg0, s32 *, 0x80) = arg1;
    if (arg1 == 1) {
        M2C_FIELD(arg0, s32 *, 0x78) = 0xFFFF;
    }
}
