#include "m2c_prelude.h"
s32 FBhvgotoArrived(s32);                             /* extern */
M2C_UNK AdvanceBhvpatrolpathGoal(void *, M2C_UNK);             /* extern */
M2C_UNK UpdateBhv();                            /* extern */

void UpdateBhvpatrolpath(void *arg0) {
    UpdateBhv();
    if (FBhvgotoArrived(M2C_FIELD(arg0, s32 *, 0x18)) != 0) {
        AdvanceBhvpatrolpathGoal(arg0, 0);
    }
}
