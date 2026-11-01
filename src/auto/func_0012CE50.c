#include "m2c_prelude.h"
M2C_UNK PvAllocSlotheapClear(M2C_UNK *);                   /* extern */
extern M2C_UNK D_003E1254;

/** @sly2-annot addr=0x12CE50
 * Purpose: Allocates an ACT-family object from its slot pool (PvAllocSlotheapClear)
 * Info: proposed name PactAlloc (T2); TU act
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/animation.md#act-0x12b3b0-0x12e600
 * @end-sly2-annot */
void func_0012CE50(void) {
    PvAllocSlotheapClear(&D_003E1254);
}
