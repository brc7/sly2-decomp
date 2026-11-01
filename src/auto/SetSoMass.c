#include "m2c_prelude.h"
M2C_UNK AdjustSoMomint(f32);                         /* extern */

void SetSoMass(void *arg0, f32 fparg0) {
    f32 temp_f0;
    f32 temp_f12;
    void *temp_v0;

    temp_v0 = M2C_FIELD(arg0, void **, 0xE8);
    temp_f0 = M2C_FIELD(temp_v0, f32 *, 0xD0);
    temp_f12 = fparg0 / temp_f0;
    M2C_FIELD(temp_v0, f32 *, 0xD0) = (f32) (temp_f0 * temp_f12);
    AdjustSoMomint(temp_f12);
}
