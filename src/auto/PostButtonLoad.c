#include "m2c_prelude.h"
M2C_UNK PostBtnLoad(s32);                         /* extern */
M2C_UNK SetButtonButtons(s32, M2C_UNK);                /* extern */
M2C_UNK PostSoLoad();                            /* extern */

void PostButtonLoad(s32 arg0) {
    PostSoLoad();
    PostBtnLoad(arg0 + 0x150);
    SetButtonButtons(arg0, 0);
}
