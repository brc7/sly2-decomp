#include "m2c_prelude.h"
M2C_UNK ReturnJtToSafePlace(s32, M2C_UNK);                /* extern */

void ReturnJtToSafePlaceCb(s32 *arg0) {
    ReturnJtToSafePlace(*arg0, 0);
}
