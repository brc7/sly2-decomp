#include "m2c_prelude.h"
M2C_UNK PvAllocSlotheapClear(M2C_UNK *);                   /* extern */
extern M2C_UNK D_003DD074;

/** @sly2-annot addr=0x1FCCF0
 * Purpose: Allocate from SNDMGR 0x3C0 pool (+0x34)
 * Info: proposed name PvAllocStreamsnd (T1); TU sound
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7_1F8DD8+verdicts_tu_r7r8
 * See: docs/engine/sound.md#6-key-functions, docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void PvAllocStreamsnd(void) {
    PvAllocSlotheapClear(&D_003DD074);
}
