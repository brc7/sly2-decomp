#include "m2c_prelude.h"
/* Warning: missing "jr $ra" in last block of func_0011E020 (initial). */

/* Warning: missing "jr $ra" in last block of func_0011EBA0 (initial). */

/* Warning: missing "jr $ra" in last block of func_0011F278 (initial). */

extern s128 D_002A2DA0;
extern s128 D_002A2DD0;

/** @sly2-annot addr=0x1202D0
 * Purpose: Splice builtin "print": returns Void (tag 0x24): print compiled out
 * Info: proposed name RefOpPrint (T1); TU bif
 * Confidence: high [T1 0.93] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
s32 RefOpPrint__FiP4CRefP6CFrame(s32 arg0) {
    func_0012A0B0(arg0, 0x24);
    return arg0;
}
