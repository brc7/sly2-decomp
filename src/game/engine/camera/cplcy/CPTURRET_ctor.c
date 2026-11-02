#include "m2c_prelude.h"
M2C_UNK CPLCY_ctor();                            /* extern */
extern M2C_UNK D_002CD3C0;

/** @sly2-annot addr=0x1518C8
 * Purpose: CPTURRET ctor (vtable D_002CD3C0)
 * Info: proposed name CPTURRET_ctor (T1); class CPTURRET; TU cplcy
 * Confidence: mid (draft) [T1 0.65] - evidence: semantic review; sources: r2_131F70
 * See: docs/reference/functions/camera.md#cplcy-0x14fca8-0x1520c8
 * @end-sly2-annot */
void *CPTURRET_ctor(void *arg0) {
    CPLCY_ctor();
    M2C_FIELD(arg0, M2C_UNK **, 0xC) = &D_002CD3C0;
    return arg0;
}
