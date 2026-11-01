#include "m2c_prelude.h"
M2C_UNK UpdateLo();                            /* extern */

/** @sly2-annot addr=0x1398E0
 * Purpose: BLIPG vt16 Update: just UpdateLo
 * Info: proposed name UpdateBlipg (T1); class BLIPG slot16; TU blip
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/particles-effects.md#blip-0x138f08-0x13a9e8
 * @end-sly2-annot */
void UpdateBlipg(void) {
    UpdateLo();
}
