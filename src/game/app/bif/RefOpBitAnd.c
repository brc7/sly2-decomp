#include "m2c_prelude.h"
extern f32 D_002DDED0;
extern M2C_UNK D_002DDF40;
extern M2C_UNK D_002DFC00;
extern M2C_UNK D_003D4A60;
extern M2C_UNK D_003DA0C0;
extern M2C_UNK D_003DD040;
extern M2C_UNK D_003DD490;
extern M2C_UNK D_003E0EA8;
extern M2C_UNK D_004520A8;

/** @sly2-annot addr=0x1255F0
 * Purpose: Bitwise AND of all S32 list args (init -1)
 * Info: proposed name RefOpBitAnd (T1); TU bif
 * Notes: splice builtin 135 bit-and~ -> bit-and~ (0.80): Body: bitwise AND of the int arguments (variadic).
 *   Scripts: (not (= (bit-and~ a b) 0)), (bit-and~ (geto x 'CID98+0x168:935) 1)
 * Confidence: high [T1 0.80] - evidence: semantic review; sources: r1_116918+top50_round(splice builtin 135)
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
s32 RefOpBitAnd(void *arg0, s32 arg1, void *arg2) {
    s32 temp_v0;
    s32 var_a1;
    void *var_v1;

    var_a1 = -1;
    var_v1 = NULL;
    if (M2C_FIELD(arg2, s32 *, 0) != 0) {
        var_v1 = M2C_FIELD(arg2, void **, 4);
    }
    if (var_v1 != NULL) {
        do {
            temp_v0 = M2C_FIELD(var_v1, s32 *, 0xC);
            var_v1 = M2C_FIELD(var_v1, void **, 0x10);
            var_a1 &= temp_v0;
        } while (var_v1 != NULL);
    }
    M2C_FIELD(arg0, s32 *, 4) = var_a1;
    M2C_FIELD(arg0, s32 *, 0) = 0x15;
    return (s32) arg0;
}
