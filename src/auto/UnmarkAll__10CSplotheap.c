#include "m2c_prelude.h"
/** @sly2-annot addr=0x14BF18
 * Purpose: Sets ijbcCur = 0 on every code
 * Info: proposed name _ResetCodes (EXISTING); TU cheat
 * Notes: agent/verifier proposes _ResetCodes for this existing name (see existing_name_changes.csv) | RENAME
 *   proposed -> _ResetCodes (existing_name_changes.csv)
 * Confidence: high [EXISTING 0.80] - evidence: semantic review; arbitration; pre-existing symbol in
 *   symbol_addrs.txt; sources: semantic_tu_docs/cheat+arbitration
 * See: docs/engine/main-loop-startup.md#8-key-functions,
 *   docs/reference/functions/main-loop-startup.md#cheat-0x14bea8-0x14c548
 * @end-sly2-annot */
void UnmarkAll__10CSplotheap(void **arg0) {
    void *var_a0;

    var_a0 = *arg0;
    if (var_a0 != NULL) {
        do {
            M2C_FIELD(var_a0, s32 *, 0x10) = 0;
            var_a0 = M2C_FIELD(var_a0, void **, 0x18);
        } while (var_a0 != NULL);
    }
}
