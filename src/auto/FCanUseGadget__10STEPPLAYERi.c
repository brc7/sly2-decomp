#include "m2c_prelude.h"
/** @sly2-annot addr=0x141C00
 * Purpose: vt99 default: returns 0 (no gadgets usable)
 * Info: proposed name STEPPLAYER_FCanUseGadget (T1); class STEPPLAYER slot99; TU stepplayer
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
s32 FCanUseGadget__10STEPPLAYERi(void) {
    return 0;
}
