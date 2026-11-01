#include "m2c_prelude.h"
M2C_UNK func_00175488(s32, M2C_UNK);                /* extern */
M2C_UNK StartBhv();                            /* extern */
M2C_UNK SetBhvBusy(s32);                         /* extern */

void func_00175410(s32 arg0) {
    StartBhv();
    func_00175488(arg0, 0);
    SetBhvBusy(arg0);
}
