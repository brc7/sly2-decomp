#include "m2c_prelude.h"
M2C_UNK LoadBtn(s32, s32);                    /* extern */
M2C_UNK LoadSoFromBrx();                            /* extern */

void LoadButtonFromBrx(s32 arg0) {
    LoadSoFromBrx();
    LoadBtn(arg0 + 0x150, arg0);
}
