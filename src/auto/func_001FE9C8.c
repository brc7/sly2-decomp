#include "m2c_prelude.h"
M2C_UNK func_00232250(s32);                         /* extern */

/** @sly2-annot addr=0x1FE9C8
 * Purpose: snd_SetPlaybackMode(mode) if changed (mono/stereo option)
 * Info: proposed name SetSndmgrPlaybackMode (T1); TU sound
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_001FE9C8(void *arg0, s32 arg1) {
    if (M2C_FIELD(arg0, s32 *, 0x104) != arg1) {
        M2C_FIELD(arg0, s32 *, 0x104) = arg1;
        func_00232250(arg1);
    }
}
