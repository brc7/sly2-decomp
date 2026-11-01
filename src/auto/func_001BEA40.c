#include "m2c_prelude.h"
M2C_UNK func_00231100();                            /* extern */

/** @sly2-annot addr=0x1BEA40
 * Purpose: CID 45 vt7: InitXfm, fov/near/far defaults 1, 4, 1000
 * Info: proposed name InitCid45 (T2); class CID45 slot7; TU tu_1BE398
 * Confidence: low (draft) [T2 0.55] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/rendering.md#tu_1be398-0x1be398-0x1bf110
 * @end-sly2-annot */
void func_001BEA40(void *arg0) {
    func_00231100();
    M2C_FIELD(arg0, f32 *, 0x11C) = 1.0f;
    M2C_FIELD(arg0, f32 *, 0x120) = 4.0f;
    M2C_FIELD(arg0, f32 *, 0x124) = 1000.0f;
}
