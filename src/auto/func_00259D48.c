#include "m2c_prelude.h"
/** @sly2-annot addr=0x259D48
 * Purpose: p->xF64->xEC = v [game linkonce (inline/template)]
 * Info: proposed name SetF64_EC (T2); TU game_linkonce
 * Confidence: low (draft) [T2 0.15] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
void func_00259D48(void *arg0, s32 arg1) {
    M2C_FIELD(M2C_FIELD(arg0, void **, 0xF64), s32 *, 0xEC) = arg1;
}
