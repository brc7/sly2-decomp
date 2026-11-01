#include "m2c_prelude.h"
M2C_UNK FGrfHasSgf(s64);                         /* extern */

void FStepguardHasSgf(void *arg0) {
    FGrfHasSgf(M2C_FIELD(arg0, s64 *, 0x2C0));
}
