#include "m2c_prelude.h"
M2C_UNK func_00156780();                            /* extern */
extern f32 D_002DDED0;

/** @sly2-annot addr=0x156728
 * Purpose: Removes the noise once its template lifetime (+0x60, if >=0) has elapsed
 * Info: proposed name ExpireNoise (T2); TU noise
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/guards-ai.md#noise-0x1564e8-0x157200
 * @end-sly2-annot */
void func_00156728(void *arg0) {
    f32 temp_f2;

    temp_f2 = M2C_FIELD(M2C_FIELD(arg0, void **, 0x14), f32 *, 0x60);
    if (temp_f2 >= 0.0f) {
        if ((M2C_FIELD(arg0, f32 *, 0x4C) + temp_f2) < D_002DDED0) {
            func_00156780();
        }
    }
}
