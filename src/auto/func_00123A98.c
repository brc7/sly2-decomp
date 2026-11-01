#include "m2c_prelude.h"
s32 func_00210AE0();                                /* extern */

/** @sly2-annot addr=0x123A98
 * Purpose: S32 = table D_003E1C44[cur] (0x210AE0) or -1
 * Info: proposed name RefOpCurrentWorld (T1); TU bif
 * Notes: v3: superseded RefOpGetLoadFilterMappedB | splice builtin 99 load-filter-world~ -> current-world~
 *   (0.65): Body returns Int(NWorldCur()) = g_anWorldOfLevel[IlevelCur()] or -1. Scripts: (case
 *   (load-filter-world~) ((1) 33) ((2) 34) ...) and (= (load-filter-world~) -1)
 * Confidence: mid (draft) [T1 0.65] - evidence: semantic review; sources: r1_116918+top50_round(splice
 *   builtin 99)
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
void *func_00123A98(void *arg0) {
    M2C_FIELD(arg0, s32 *, 4) = func_00210AE0();
    M2C_FIELD(arg0, s32 *, 0) = 0x15;
    return arg0;
}
