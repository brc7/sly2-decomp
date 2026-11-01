#include "m2c_prelude.h"
M2C_UNK PostLoadJtModeHud(s32);                         /* extern */

/** @sly2-annot addr=0x19CC30
 * Purpose: PostLoad: bind the mode HUD widgets (0x19D540)
 * Info: proposed name PostLoadJtMode (T1); TU mode
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638,
 *   docs/reference/functions/player.md#mode-0x19cbf8-0x19de10
 * @end-sly2-annot */
void PostLoadJtMode(s32 arg0) {
    PostLoadJtModeHud(arg0 + 0x24);
}
