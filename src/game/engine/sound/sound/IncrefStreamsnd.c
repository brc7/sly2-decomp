#include "m2c_prelude.h"
/** @sly2-annot addr=0x1FCC90
 * Purpose: vt1: ++cref (u16 +0x4C)
 * Info: proposed name IncrefStreamsnd (T1); class STREAMSND; TU sound
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void IncrefStreamsnd(void *arg0) {
    M2C_FIELD(arg0, u16 *, 0x4C) = (u16) (M2C_FIELD(arg0, u16 *, 0x4C) + 1);
}
