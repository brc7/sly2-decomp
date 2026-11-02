#include "m2c_prelude.h"
M2C_UNK Decref__4CRef(M2C_UNK *);                   /* extern */
extern M2C_UNK D_0027A10C;
extern M2C_UNK D_0027A1D8;
extern M2C_UNK D_002BAAE8;
extern f32 D_002DDED0;
extern M2C_UNK D_003DD490;
extern f32 *D_003E0E80;
extern M2C_UNK D_003E1144;
extern M2C_UNK D_004520A8;

/** @sly2-annot addr=0x132600
 * Purpose: If the file record kind (+8) == 2 open the async stream on its sector range (FOpenSector(+0,+4)),
 *   else 0
 * Info: proposed name FOpenBasFromCdFile (T2); TU bas
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r2b_rest
 * See: docs/formats/disc-records.md#2-the-read-path,
 *   docs/reference/functions/core-library.md#bas-0x132570-0x132b00
 * @end-sly2-annot */
s32 func_00132600(s32 arg0, s32 arg1) {
    s32 var_a2;

    var_a2 = 0;
    if (M2C_FIELD(arg1, s32 *, 8) == 2) {
        var_a2 = FOpenSector__18CBinaryAsyncStreamUiUi(arg0, M2C_FIELD(arg1, s32 *, 0), M2C_FIELD(arg1, s32 *, 4));
    }
    return var_a2;
}
