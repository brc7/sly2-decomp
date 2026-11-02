#include "m2c_prelude.h"
M2C_UNK func_0011CD98(M2C_UNK *, s32);              /* extern */
extern M2C_UNK D_003DD074;

/** @sly2-annot addr=0x1FCD10
 * Purpose: Free to SNDMGR 0x3C0 pool
 * Info: proposed name FreeStreamsnd (T1); TU sound
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void FreeStreamsnd(s32 arg0) {
    func_0011CD98(&D_003DD074, arg0);
}
