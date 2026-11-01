#include "m2c_prelude.h"
M2C_UNK InitXfm();                            /* extern */

void InitJmt(void *arg0) {
    InitXfm();
    M2C_FIELD(arg0, f32 *, 0xE8) = 100.0f;
}
