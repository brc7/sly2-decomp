#include "m2c_prelude.h"
M2C_UNK AddBtnOffAseg(s32);                         /* extern */

void AddVolbtnOffAseg(s32 arg0) {
    AddBtnOffAseg(arg0 + 0x130);
}
