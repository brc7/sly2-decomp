#include "m2c_prelude.h"
M2C_UNK InitGrabspot(s32);                         /* extern */
M2C_UNK InitSo();                            /* extern */
M2C_UNK func_001F7918(void *, M2C_UNK, M2C_UNK);    /* extern */

void InitBarrel(void *arg0) {
    InitSo();
    InitGrabspot(arg0 + 0x150);
    M2C_FIELD(arg0, s32 *, 0x17C) = 0;
    M2C_FIELD(arg0, f32 *, 0x158) = 300.0f;
    M2C_FIELD(arg0, s32 *, 0x18C) = 1;
    func_001F7918(arg0, 0x03FFFFFF, 8);
    func_001F7918(arg0, 0x03188001, 0);
}
