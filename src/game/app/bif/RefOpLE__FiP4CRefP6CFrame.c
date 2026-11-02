#include "m2c_prelude.h"
/* Warning: missing "jr $ra" in last block of func_0011E020 (initial). */

/* Warning: missing "jr $ra" in last block of func_0011EBA0 (initial). */

/* Warning: missing "jr $ra" in last block of func_0011F278 (initial). */

M2C_UNK func_001291A8(s32, void *);                 /* extern */
M2C_UNK func_00129EC8(void *);                      /* extern */
M2C_UNK SetTag__4CRef4TAGK(M2C_UNK, s32);                /* extern */
extern s128 D_002A2DA0;
extern s128 D_002A2DD0;

/** @sly2-annot addr=0x1204B8
 * Purpose: Splice builtin "<=": RefCmp(aref,1) <=
 * Info: proposed name RefOpLE (T1); TU bif
 * Confidence: high [T1 0.93] - evidence: static analysis; sources: r1_116918+verdicts_A1
 * See: docs/engine/splice-scripting.md#7-key-functions,
 *   docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
s32 RefOpLE__FiP4CRefP6CFrame(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    RefCmp__FP4CRef4CMPK(arg0, arg2, 1, arg3, arg4);
    return arg0;
}
