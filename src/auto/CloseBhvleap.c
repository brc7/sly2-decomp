#include "m2c_prelude.h"
M2C_UNK CloseBhvanim();                            /* extern */
M2C_UNK ClearBhvBusy(s32);                         /* extern */

void CloseBhvleap(s32 arg0) {
    CloseBhvanim();
    ClearBhvBusy(arg0);
}
