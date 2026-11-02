#include "m2c_prelude.h"
M2C_UNK func_001D7C80(M2C_UNK *);                   /* extern */
extern M2C_UNK D_003E0EA8;

/** @sly2-annot addr=0x1AC570
 * Purpose: Thin wrapper over ResetGsIfSchemaChanged for a slot
 * Info: proposed name ResetSaveSlot (T2); TU memcard
 * Notes: names_v2 C proposed Clear__13CFileLocation (0.200)
 * Confidence: low (draft) [T2 0.35] - evidence: static naming pass; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
s32 func_001AC570(void) {
    func_001D7C80(&D_003E0EA8);
    return 0;
}
