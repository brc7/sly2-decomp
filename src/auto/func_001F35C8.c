#include "m2c_prelude.h"
M2C_UNK func_001F35F8(f32);                         /* extern */

/** @sly2-annot addr=0x1F35C8
 * Purpose: Scale SOPHYS mass (+0xD0) to new value and scale inertia via 0x1F35F8
 * Info: proposed name SetSoMass (T1); TU so
 * Notes: names_v2 C proposed SetSoCameraStyle__FP2SO3CMK (0.167)
 * Confidence: mid (draft) [T1 0.55] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F35C8(void *arg0, f32 fparg0) {
    f32 temp_f0;
    f32 temp_f12;
    void *temp_v0;

    temp_v0 = M2C_FIELD(arg0, void **, 0xE8);
    temp_f0 = M2C_FIELD(temp_v0, f32 *, 0xD0);
    temp_f12 = fparg0 / temp_f0;
    M2C_FIELD(temp_v0, f32 *, 0xD0) = (f32) (temp_f0 * temp_f12);
    func_001F35F8(temp_f12);
}
