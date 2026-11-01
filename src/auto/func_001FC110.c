#include "m2c_prelude.h"
M2C_UNK func_0011CD98(M2C_UNK *, s32);              /* extern */
extern M2C_UNK D_003DD05C;

/** @sly2-annot addr=0x1FC110
 * Purpose: Free to SNDMGR 0x70-byte pool
 * Info: proposed name FreeBanksnd (T1); TU sound
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_001FC110(s32 arg0) {
    func_0011CD98(&D_003DD05C, arg0);
}
