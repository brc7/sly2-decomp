#include "m2c_prelude.h"
M2C_UNK CPLCY_ctor();                            /* extern */
extern M2C_UNK D_002CD4E0;

/** @sly2-annot addr=0x14FDE0
 * Purpose: CPLOOK ctor (vtable D_002CD4E0): rZoomMax(+0x28)=7.0, psndZoom/psndUnk38 = 0
 * Info: proposed name CPLOOK_ctor (T1); class CPLOOK; TU cplcy
 * Notes: runtime5a #9/#11: CM+0x450 CPLOOK prio 7 = binocucom camera for all three characters
 * Confidence: high [T1 0.85] - evidence: runtime (save-state RAM); semantic review; sources:
 *   r2_131F70+runtime
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cplcy-0x14fca8-0x1520c8
 * @end-sly2-annot */
void *CPLOOK_ctor(void *arg0) {
    CPLCY_ctor();
    M2C_FIELD(arg0, M2C_UNK **, 0xC) = &D_002CD4E0;
    M2C_FIELD(arg0, f32 *, 0x28) = 7.0f;
    M2C_FIELD(arg0, s32 *, 0x34) = 0;
    M2C_FIELD(arg0, s32 *, 0x38) = 0;
    return arg0;
}
