#include "m2c_prelude.h"
/** @sly2-annot addr=0x2068D8
 * Purpose: qsort comparator on a leading float (-1 if a<b else 1)
 * Info: proposed name NCmpGuardCandidateDist (EXISTING); TU stepguard
 * Notes: names_v2 C proposed SgnCompareSdprize__FPCvT0 (0.200) | verifier PLAUSIBLE: Existing name carried
 *   forward: 'FUN_00148910' names a different address, so it is a misleading placeholder. Body: -1 if *a < *b
 *   else 1 (float comparator). No direct callers (passed as a function point | RENAME proposed ->
 *   NCmpGuardCandidateDist (existing_name_changes.csv)
 * Confidence: high [EXISTING 0.55] - evidence: arbitration; static naming pass; pre-existing symbol in
 *   symbol_addrs.txt; sources: r7b_guard+verdicts_followups+arbitration
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
s32 NCmpGuardCandidateDist(f32 *arg0, f32 *arg1) {
    s32 var_v0;

    var_v0 = 1;
    if (*arg0 < *arg1) {
        var_v0 = -1;
    }
    return var_v0;
}
