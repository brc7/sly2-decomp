#include "m2c_prelude.h"
M2C_UNK SetLoBusyk(s32, M2C_UNK);                /* extern */
M2C_UNK InitXfm();                            /* extern */

/** @sly2-annot addr=0x1CA788
 * Purpose: PROXY::Init (vt7): InitXfm + flag
 * Info: proposed name InitProxy (T1); class PROXY slot7; TU proxy
 * Notes: names_v2 C proposed SetAloAngularVelocityXYZ__FP3ALOfff (0.438)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r5_1AE130
 * See: docs/engine/object-model.md#9-key-functions,
 *   docs/reference/functions/object-model.md#proxy-0x1c9940-0x1cb908
 * @end-sly2-annot */
void InitProxy(s32 arg0) {
    InitXfm();
    SetLoBusyk(arg0, 0);
}
