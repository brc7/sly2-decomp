#include "m2c_prelude.h"
M2C_UNK UpdateSwIllum();                            /* extern */

void SetSwIllum(void *arg0, f32 fparg0) {
    M2C_FIELD(arg0, f32 *, 0x3E20) = fparg0;
    UpdateSwIllum();
}
