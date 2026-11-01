#include "m2c_prelude.h"
s32 rand();                                         /* extern */

/** @sly2-annot addr=0x11D3C0
 * Purpose: rand() % 905010707
 * Info: proposed name NRand (T1); TU util
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/core-library.md#util-0x11cf60-0x11e198
 * @end-sly2-annot */
s32 NRand(void) {
    return rand() % 905010707;
}
