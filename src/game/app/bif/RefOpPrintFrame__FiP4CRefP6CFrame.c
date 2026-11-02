#include "m2c_prelude.h"
/* Warning: missing "jr $ra" in last block of func_0011E020 (initial). */

/* Warning: missing "jr $ra" in last block of func_0011EBA0 (initial). */

/* Warning: missing "jr $ra" in last block of func_0011F278 (initial). */

extern s128 D_002A2DA0;
extern s128 D_002A2DD0;

/** @sly2-annot addr=0x120300
 * Purpose: Splice builtin "print-frame": returns Void
 * Info: proposed name RefOpPrintFrame (T1); TU bif
 * Confidence: high [T1 0.93] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
s32 RefOpPrintFrame__FiP4CRefP6CFrame(s32 arg0) {
    SetTag__4CRef4TAGK(arg0, 0x24);
    return arg0;
}
