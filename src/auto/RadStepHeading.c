#include "m2c_prelude.h"
M2C_UNK atan2f(f32, f32);                           /* extern */

void RadStepHeading(void *arg0) {
    void *temp_v0;

    temp_v0 = M2C_FIELD(arg0, void **, 0x60);
    atan2f(M2C_FIELD(temp_v0, f32 *, 0x44), M2C_FIELD(temp_v0, f32 *, 0x40));
}
