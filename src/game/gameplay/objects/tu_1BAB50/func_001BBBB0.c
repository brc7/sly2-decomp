#include "m2c_prelude.h"
void *func_001BBDB0(s32, M2C_UNK);                  /* extern */
s32 func_001BBE18();                                /* extern */
M2C_UNK ShowSoGlobsById(s32, s32);                    /* extern */

/** @sly2-annot addr=0x1BBBB0
 * Purpose: Outside stage recording: ShowSoGlobsById now; else record action 2
 * Info: proposed name AddCid98ShowGlobs (T2); class CID98; TU tu_1BAB50
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/gameplay-objects.md#tu_1bab50-0x1bab50-0x1bc050
 * @end-sly2-annot */
void func_001BBBB0(s32 arg0, s32 arg1) {
    if (func_001BBE18() == 0) {
        ShowSoGlobsById(arg0, arg1);
        return;
    }
    M2C_FIELD(func_001BBDB0(arg0, 2), s32 *, 4) = arg1;
}
