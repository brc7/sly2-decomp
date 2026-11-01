#include "m2c_prelude.h"
M2C_UNK UntriggerBtn(s32);                         /* extern */

void UntriggerVolbtn(s32 arg0) {
    UntriggerBtn(arg0 + 0x130);
}
