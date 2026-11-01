#include "m2c_prelude.h"
M2C_UNK StaticInit_gc(M2C_UNK, M2C_UNK);            /* extern */

/** @sly2-annot addr=0x128B40
 * Purpose: compiler-generated global constructor stub (calls static init with (1,0xFFFF))
 * Info: proposed name _GLOBAL_$I$g_gc (T1); TU gc
 * Notes: gcc name: first global of gc TU is g_gc (r1 fix: existing _GLOBAL_$I$g_gc at 0x128118 is the frame
 *   TU stub) | name currently held by 0x128118, which existing_name_changes.csv proposes to rename |
 *   arbitration knock-on rename: CONFIRMED static: calls 0x128A00 (static init that constructs g_gc via CGc
 *   ctor 0x128138). Sly1 has _GLOBAL_$I$g_gc in gc.cpp.
 * Confidence: high [T1 0.85] - evidence: Sly 1 / Lilly reference; arbitration; sources:
 *   tu_map(structural)+r1_116918+arbitration
 * See: docs/engine/splice-scripting.md#3-the-evaluator
 * @end-sly2-annot */
void func_00128B40(void) {
    StaticInit_gc(1, 0xFFFF);
}
