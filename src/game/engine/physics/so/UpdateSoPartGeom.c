#include "m2c_prelude.h"
M2C_UNK func_001664A0(void *, void *, void *);      /* extern */
M2C_UNK func_001F4BF8(s32);                         /* extern */

/** @sly2-annot addr=0x1F38A8
 * Purpose: If part stamp != XF stamp: transform local geom to world (0x1664A0), copy stamp, mark tree dirty
 * Info: proposed name UpdateSoPartGeom (T1); TU so
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void UpdateSoPartGeom(void *arg0) {
    void *temp_a0;

    temp_a0 = M2C_FIELD(arg0, void **, 0);
    if (M2C_FIELD(arg0, s32 *, 0xA8) != M2C_FIELD(temp_a0, s32 *, 0xA4)) {
        func_001664A0(temp_a0 + 0x40, arg0 + 0x78, arg0 + 0x90);
        M2C_FIELD(arg0, s32 *, 0xA8) = (s32) M2C_FIELD(M2C_FIELD(arg0, void **, 0), s32 *, 0xA4);
        func_001F4BF8(M2C_FIELD(arg0, s32 *, 4));
    }
}
