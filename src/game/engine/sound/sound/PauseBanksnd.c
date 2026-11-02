#include "m2c_prelude.h"
M2C_UNK snd_PauseSound(s32);                         /* extern */

/** @sly2-annot addr=0x1FBEE0
 * Purpose: vt11: snd_PauseSound(handle)
 * Info: proposed name PauseBanksnd (T1); class BANKSND; TU sound
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void PauseBanksnd(void *arg0) {
    snd_PauseSound(M2C_FIELD(arg0, s32 *, 0x40));
}
