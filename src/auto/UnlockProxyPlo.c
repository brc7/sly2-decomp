#include "m2c_prelude.h"
void *func_001CB410();                              /* extern */

void UnlockProxyPlo(void) {
    void *temp_v0;

    temp_v0 = func_001CB410();
    if (temp_v0 != NULL) {
        M2C_FIELD(temp_v0, s32 *, 8) = 0;
    }
}
