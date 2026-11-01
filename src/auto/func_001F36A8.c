#include "m2c_prelude.h"
M2C_UNK func_00167B68(s32);                         /* extern */
M2C_UNK func_001A5C48();                            /* extern */
M2C_UNK func_0022FAA0(s32);                         /* extern */

/** @sly2-annot addr=0x1F36A8
 * Purpose: SO vt15: PostLoLoad, 0x22FAA0, globset post-load (0x167B68 on +0x80)
 * Info: proposed name PostSoLoad (T1); class SO slot15; TU so
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8,
 *   docs/reference/functions/physics-collision.md#water-0x228b48-0x229ef0
 * @end-sly2-annot */
void func_001F36A8(s32 arg0) {
    func_001A5C48();
    func_0022FAA0(arg0);
    func_00167B68(arg0 + 0x80);
}
