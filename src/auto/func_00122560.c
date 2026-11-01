#include "m2c_prelude.h"
M2C_UNK func_001291A8(s32, s32 *);                  /* extern */
M2C_UNK func_00129EC8(s32 *);                       /* extern */
M2C_UNK func_0012A0B0(s32, M2C_UNK);                /* extern */
extern s128 D_002A2DD0;
extern M2C_UNK D_002BBB70;
extern M2C_UNK D_004520A8;

/** @sly2-annot addr=0x122560
 * Purpose: Splice builtin "sin": RefUfo(arg,1)
 * Info: proposed name RefOpSin (T1); TU bif
 * Confidence: high [T1 0.93] - evidence: static analysis; sources: r1_116918+verdicts_tu_r1r2+verdicts_A1
 * See: docs/engine/splice-scripting.md#7-key-functions,
 *   docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
s32 func_00122560(s32 arg0, s32 arg1, s32 arg2) {
    func_001224A8(arg0, arg2, 1);
    return arg0;
}
