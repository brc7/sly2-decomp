#include "m2c_prelude.h"
M2C_UNK SetCmNearClip__FP2CMf(M2C_UNK *, f32);              /* extern */
extern M2C_UNK D_002DDF40;

/** @sly2-annot addr=0x14FCE8
 * Purpose: Default slot 3 Deactivate: pvHero=0; SetCmNearClip(saved); fovTarget=saved
 * Info: proposed name DeactivateCplcy (T1); class CPLCY; TU cplcy
 * Confidence: high [T1 0.80] - evidence: semantic review; sources: r2_131F70
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cplcy-0x14fca8-0x1520c8,
 *   docs/reference/functions/camera.md#tn-0x220d40-0x223f70
 * @end-sly2-annot */
void DeactivateCplcy(void *arg0) {
    M2C_FIELD(arg0, s32 *, 0) = 0;
    SetCmNearClip__FP2CMf(&D_002DDF40, M2C_FIELD(arg0, f32 *, 8));
    M2C_FIELD(&D_002DDF40, f32 *, 0x24) = (f32) M2C_FIELD(arg0, f32 *, 4);
}
