#include "m2c_prelude.h"
M2C_UNK func_001291A8(s32, s32 *);                  /* extern */
M2C_UNK Decref__4CRef(s32 *);                       /* extern */
M2C_UNK SetTag__4CRef4TAGK(s32, M2C_UNK);                /* extern */
extern s128 D_002A2DD0;
extern M2C_UNK D_002BBB70;
extern M2C_UNK D_004520A8;

/** @sly2-annot addr=0x1209D0
 * Purpose: Splice builtin "null-obj?": tag==0x21 && ptr==NULL
 * Info: proposed name RefOpIsNullObj (T1); TU bif
 * Confidence: high [T1 0.93] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
s32 RefOpIsNullObj__FiP4CRefP6CFrame(s32 arg0, s32 arg1, s32 arg2) {
    s32 var_a1;

    var_a1 = 0;
    if (M2C_FIELD(arg2, s32 *, 0) == 0x21) {
        var_a1 = M2C_FIELD(arg2, s32 *, 4) == 0;
    }
    M2C_FIELD(arg0, s32 *, 4) = var_a1;
    M2C_FIELD(arg0, s32 *, 0) = 0x17;
    return arg0;
}
