#include "m2c_prelude.h"
M2C_UNK AdvanceBhvpathlink(s32);                         /* extern */
M2C_UNK StartBhv();                            /* extern */
M2C_UNK SetBhvBusy(s32);                         /* extern */

void StartBhvpathlink(s32 arg0) {
    StartBhv();
    SetBhvBusy(arg0);
    AdvanceBhvpathlink(arg0);
}
