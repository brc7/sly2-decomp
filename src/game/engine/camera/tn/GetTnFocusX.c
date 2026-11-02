#include "m2c_prelude.h"
/** @sly2-annot addr=0x221980
 * Purpose: returns tnfn.vecFocus.x
 * Info: proposed name GetTnFocusX (T1); TU tn
 * Notes: verifier PLAUSIBLE: GetTnFocusRatio -> GetTnFocusX
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r8_21BB88+verdicts_tu_r7r8
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#tn-0x220d40-0x223f70
 * @end-sly2-annot */
f32 GetTnFocusX(void *arg0) {
    return M2C_FIELD(M2C_FIELD(arg0, void **, 0x118), f32 *, 0x70);
}
