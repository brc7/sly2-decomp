#include "m2c_prelude.h"
M2C_UNK InitBhvpath();                            /* extern */

void InitBhvwalkgoal(void *arg0) {
    InitBhvpath();
    M2C_FIELD(arg0, f32 *, 0xD0) = 1.0f;
}
