#include "m2c_prelude.h"
M2C_UNK SetBtnButtons(s32);                         /* extern */

void SetVolbtnButtons(s32 arg0) {
    SetBtnButtons(arg0 + 0x130);
}
