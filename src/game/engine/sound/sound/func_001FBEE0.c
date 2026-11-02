#include "m2c_prelude.h"
M2C_UNK func_002323A8(s32);                         /* extern */

/** @sly2-annot addr=0x1FBEE0
 * Purpose: vt11: snd_PauseSound(handle)
 * Info: proposed name PauseBanksnd (T1); class BANKSND; TU sound
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_001FBEE0(void *arg0) {
    func_002323A8(M2C_FIELD(arg0, s32 *, 0x40));
}
