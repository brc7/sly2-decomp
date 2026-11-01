#include "m2c_prelude.h"
extern s128 D_002A2DD0;
extern M2C_UNK D_002BBB70;
extern M2C_UNK D_004520A8;

/** @sly2-annot addr=0x120D28
 * Purpose: Splice builtin "length": count of list nodes -> S32
 * Info: proposed name RefOpLength (T1); TU bif
 * Confidence: high [T1 0.93] - evidence: static analysis; sources: r1_116918+verify_side
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
s32 RefOpLength__FiP4CRefP6CFrame(void *arg0, s32 arg1, void *arg2) {
    s32 var_a1;
    void *var_v1;

    var_a1 = 0;
    if (M2C_FIELD(arg2, s32 *, 0) == 0x1F) {
        var_v1 = M2C_FIELD(arg2, void **, 4);
        if (var_v1 != NULL) {
            do {
                var_v1 = M2C_FIELD(var_v1, void **, 0x10);
                var_a1 += 1;
            } while (var_v1 != NULL);
        }
    }
    M2C_FIELD(arg0, s32 *, 4) = var_a1;
    M2C_FIELD(arg0, s32 *, 0) = 0x15;
    return (s32) arg0;
}
