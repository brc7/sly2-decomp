#include "m2c_prelude.h"
M2C_UNK func_0011CD98(M2C_UNK *, s32);              /* extern */
extern M2C_UNK D_003E1254;

/** @sly2-annot addr=0x12CE70
 * Purpose: Returns an ACT-family object to its pool (FreeSlotheapPv)
 * Info: proposed name FreeAct (T2); TU act
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/animation.md#act-0x12b3b0-0x12e600
 * @end-sly2-annot */
void func_0012CE70(s32 arg0) {
    func_0011CD98(&D_003E1254, arg0);
}
