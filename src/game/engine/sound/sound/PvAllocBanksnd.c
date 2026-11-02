#include "m2c_prelude.h"
M2C_UNK PvAllocSlotheapClear(M2C_UNK *);                   /* extern */
extern M2C_UNK D_003DD05C;

/** @sly2-annot addr=0x1FC0F0
 * Purpose: Allocate from SNDMGR 0x70-byte pool (+0x1C)
 * Info: proposed name PvAllocBanksnd (T1); TU sound
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void PvAllocBanksnd(void) {
    PvAllocSlotheapClear(&D_003DD05C);
}
