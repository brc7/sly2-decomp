#include "m2c_prelude.h"
/** @sly2-annot addr=0x1C4A48
 * Purpose: Effective current PO: override SW+0x3F00 if set else SW+0x3EFC
 * Info: proposed name PpoCur (T1); TU po
 * Notes: verifier CORRECT: PpoActive -> PpoCur | runtime3: SW+0x3EFC is the current player (Murray in
 *   S32-S35)
 * Confidence: high [T1 0.90] - evidence: runtime (save-state RAM); sources:
 *   r5_1AE130+verdicts_tu_r3r5+runtime
 * See: docs/engine/camera.md#5-key-functions, docs/engine/player.md#11-key-functions,
 *   docs/reference/functions/camera.md#cplcy-0x14fca8-0x1520c8
 * @end-sly2-annot */
s32 PpoCur(void *arg0) {
    s32 var_v0;

    var_v0 = M2C_FIELD(arg0, s32 *, 0x3F00);
    if (var_v0 == 0) {
        var_v0 = M2C_FIELD(arg0, s32 *, 0x3EFC);
    }
    return var_v0;
}
