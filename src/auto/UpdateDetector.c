#include "m2c_prelude.h"
M2C_UNK UpdateDetectorTargets(s32);                         /* extern */
M2C_UNK UpdateLo();                            /* extern */

void UpdateDetector(s32 arg0) {
    UpdateLo();
    UpdateDetectorTargets(arg0);
}
