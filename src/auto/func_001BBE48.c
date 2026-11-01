#include "m2c_prelude.h"
s32 func_001BBE18();                                /* extern */

/** @sly2-annot addr=0x1BBE48
 * Purpose: !FCid98StageValid
 * Info: proposed name FCid98Done (T2); class CID98; TU tu_1BAB50
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/gameplay-objects.md#tu_1bab50-0x1bab50-0x1bc050
 * @end-sly2-annot */
s32 func_001BBE48(void) {
    return func_001BBE18() == 0;
}
