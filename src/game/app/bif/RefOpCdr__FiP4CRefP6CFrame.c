#include "m2c_prelude.h"
extern s128 D_002A2DD0;
extern M2C_UNK D_002BBB70;
extern M2C_UNK D_004520A8;

/** @sly2-annot addr=0x120C00
 * Purpose: Splice builtin "cdr": CRef(node->pnext) or Nil
 * Info: proposed name RefOpCdr (T1); TU bif
 * Notes: names_v2 C proposed RefOpCar__FiP4CRefP6CFrame (0.638)
 * Confidence: high [T1 0.93] - evidence: static naming pass; sources: r1_116918+verify_side
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
s32 RefOpCdr__FiP4CRefP6CFrame(s32 arg0, s32 arg1, void *arg2) {
    void *temp_a1;

    temp_a1 = M2C_FIELD(M2C_FIELD(arg2, void **, 4), void **, 0x10);
    if (temp_a1 == NULL) {
        SetTag__4CRef4TAGK(arg0, 0);
    } else {
        CRef_ctor_CPair(arg0, temp_a1);
    }
    return arg0;
}
