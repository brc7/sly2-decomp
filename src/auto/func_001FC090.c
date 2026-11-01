#include "m2c_prelude.h"
/** @sly2-annot addr=0x1FC090
 * Purpose: vt1: ++cref (u16 +0x4C)
 * Info: proposed name IncrefBanksnd (T1); class BANKSND; TU sound
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_001FC090(void *arg0) {
    M2C_FIELD(arg0, u16 *, 0x4C) = (u16) (M2C_FIELD(arg0, u16 *, 0x4C) + 1);
}
