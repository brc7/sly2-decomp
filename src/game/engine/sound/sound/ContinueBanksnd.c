#include "m2c_prelude.h"
M2C_UNK snd_ContinueSound(s32);                         /* extern */

/** @sly2-annot addr=0x1FBF00
 * Purpose: vt12: snd_ContinueSound(handle)
 * Info: proposed name ContinueBanksnd (T1); class BANKSND; TU sound
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void ContinueBanksnd(void *arg0) {
    snd_ContinueSound(M2C_FIELD(arg0, s32 *, 0x40));
}
