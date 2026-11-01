#include "m2c_prelude.h"
/** @sly2-annot addr=0x1FD070
 * Purpose: Index of a sound entry (ptr - base)/4
 * Info: proposed name IsndFromPsnd (T1); TU sound
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
s32 func_001FD070(void *arg0, s32 arg1) {
    return (s32) (arg1 - M2C_FIELD(arg0, s32 *, 0xFC)) >> 2;
}
