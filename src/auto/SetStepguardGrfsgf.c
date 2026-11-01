#include "m2c_prelude.h"
M2C_UNK func_001F1900();                            /* extern */

void SetStepguardGrfsgf(void *arg0, s64 arg1) {
    M2C_FIELD(arg0, s64 *, 0x2C0) = arg1;
    func_001F1900();
}
