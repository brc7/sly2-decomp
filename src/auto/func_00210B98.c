#include "m2c_prelude.h"
extern s32 D_003E1C50;

/** @sly2-annot addr=0x210B98
 * Purpose: &g_arecWorldEntry[iworld] (64 bytes) if valid, else NULL
 * Info: proposed name PrecWorldEntry (T1); TU sw
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7_1F8DD8+verdicts_tu_r7r8
 * See: docs/engine/world-sw.md#10-key-functions,
 *   docs/reference/functions/main-loop-startup.md#cheat-0x14bea8-0x14c548,
 *   docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void *func_00210B98(s32 arg0) {
    s32 var_v0;
    void *temp_a0;

    if (arg0 == -1) {
        return NULL;
    }
    temp_a0 = D_003E1C50 + (arg0 << 6);
    var_v0 = 0;
    if (temp_a0 != NULL) {
        var_v0 = M2C_FIELD(temp_a0, s32 *, 4) != M2C_FIELD(temp_a0, s32 *, 0x3C);
    }
    return (var_v0 == 0) ? NULL : temp_a0;
}
