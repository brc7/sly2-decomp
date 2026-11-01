#include "m2c_prelude.h"
extern s32 D_002C2D8C;

/** @sly2-annot addr=0x1D5498
 * Purpose: If rumble mode D_002C2D8C == 0 set it to 1 (follow save setting)
 * Info: proposed name EnsureRumbleModeDefault (EXISTING); TU rumble
 * Notes: agent/verifier proposes EnsureRumbleModeDefault for this existing name (see
 *   existing_name_changes.csv) | RENAME proposed -> EnsureRumbleModeDefault (existing_name_changes.csv)
 * Confidence: high [EXISTING 0.50] - evidence: arbitration; pre-existing symbol in symbol_addrs.txt; sources:
 *   r6_1D5150+arbitration
 * See: docs/engine/main-loop-startup.md#8-key-functions,
 *   docs/reference/functions/main-loop-startup.md#rumble-0x1d5150-0x1d57a8
 * @end-sly2-annot */
void RumbleUnknown1__Fv(void) {
    if (D_002C2D8C == 0) {
        D_002C2D8C = 1;
    }
}
