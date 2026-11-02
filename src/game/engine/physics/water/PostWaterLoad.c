#include "m2c_prelude.h"
M2C_UNK PostSoLoad();                            /* extern */

/** @sly2-annot addr=0x228BF8
 * Purpose: calls SO PostLoad 0x1F36A8
 * Info: proposed name PostWaterLoad (T1); class WATER slot15; TU water
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/physics-collision.md#water-0x228b48-0x229ef0
 * @end-sly2-annot */
void PostWaterLoad(void) {
    PostSoLoad();
}
