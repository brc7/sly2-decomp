#include "m2c_prelude.h"
extern s128 D_002A2DD0;
extern M2C_UNK D_002BBB70;
extern M2C_UNK D_004520A8;

/** @sly2-annot addr=0x120BD0
 * Purpose: Splice builtin "car": copy of node->ref
 * Info: proposed name RefOpCar (T1); TU bif
 * Confidence: high [T1 0.93] - evidence: static analysis; sources: r1_116918+verify_side
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
s32 func_00120BD0(s32 arg0, s32 arg1, void *arg2) {
    func_001291A8(arg0, M2C_FIELD(arg2, s32 *, 4) + 8);
    return arg0;
}
