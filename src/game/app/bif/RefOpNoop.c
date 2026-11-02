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

/** @sly2-annot addr=0x1255C0
 * Purpose: Varargs builtin that returns Void
 * Info: proposed name RefOpNoop (T1); TU bif
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r1_116918+verdicts_B
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
s32 RefOpNoop(s32 arg0) {
    func_0012A0B0(arg0, 0x24);
    return arg0;
}
