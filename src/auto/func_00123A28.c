#include "m2c_prelude.h"
s32 func_00210A88();                                /* extern */

/** @sly2-annot addr=0x123A28
 * Purpose: S32 of global D_003E1110 (0x210A88) = current load-filter/job id used by FMatchLoadFilter
 * Info: proposed name RefOpCurrentLevel (T1); TU bif
 * Notes: v3: superseded RefOpGetLoadFilterCur | splice builtin 97 load-filter~ -> current-level~ (0.70): Body
 *   returns CRef Int(IlevelCur()) = D_003E1110 (current level index; siblings NEpisodeCur/NWorldCur).
 *   Scripts: (case (load-filter~) ...) and (= (load-filter~)
 * Confidence: mid (draft) [T1 0.70] - evidence: semantic review; sources: r1_116918+top50_round(splice
 *   builtin 97)
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
void *func_00123A28(void *arg0) {
    M2C_FIELD(arg0, s32 *, 4) = func_00210A88();
    M2C_FIELD(arg0, s32 *, 0) = 0x15;
    return arg0;
}
