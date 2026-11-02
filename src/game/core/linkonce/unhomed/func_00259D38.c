#include "m2c_prelude.h"
/** @sly2-annot addr=0x259D38
 * Purpose: p->xF64->xE8 = v [game linkonce (inline/template)]
 * Info: proposed name SetF64_E8 (T2); TU game_linkonce
 * Confidence: low (draft) [T2 0.15] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
void func_00259D38(void *arg0, s32 arg1) {
    M2C_FIELD(M2C_FIELD(arg0, void **, 0xF64), s32 *, 0xE8) = arg1;
}
