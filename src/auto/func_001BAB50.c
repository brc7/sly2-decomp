#include "m2c_prelude.h"
M2C_UNK InitSo();                            /* extern */
M2C_UNK func_001F7918(s32, M2C_UNK, M2C_UNK);       /* extern */

void func_001BAB50(s32 arg0) {
    InitSo();
    func_001F7918(arg0, 0x03FFFFFF, 8);
    func_001F7918(arg0, 0x80001, 0);
}
