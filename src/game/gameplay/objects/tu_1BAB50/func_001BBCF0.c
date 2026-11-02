#include "m2c_prelude.h"
void *func_001BBDB0(s32, M2C_UNK);                  /* extern */
s32 func_001BBE18();                                /* extern */
M2C_UNK func_001F6B48(s32, s32);                    /* extern */

/** @sly2-annot addr=0x1BBCF0
 * Purpose: HideSoPartsById now or record action 5
 * Info: proposed name AddCid98HideParts (T2); class CID98; TU tu_1BAB50
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/gameplay-objects.md#tu_1bab50-0x1bab50-0x1bc050
 * @end-sly2-annot */
void func_001BBCF0(s32 arg0, s32 arg1) {
    if (func_001BBE18() == 0) {
        func_001F6B48(arg0, arg1);
        return;
    }
    M2C_FIELD(func_001BBDB0(arg0, 5), s32 *, 4) = arg1;
}
