#include "m2c_prelude.h"
M2C_UNK SetPoCtlState();                            /* extern */
M2C_UNK func_001D7480(s32, M2C_UNK);                /* extern */

void func_001D7418(s32 arg0, s32 arg1) {
    SetPoCtlState();
    switch (arg1) {                                 /* irregular */
    case 0:
        func_001D7480(arg0, 0);
        return;
    case 1:
        func_001D7480(arg0, 1);
        break;
    }
}
