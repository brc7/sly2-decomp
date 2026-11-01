#include "m2c_prelude.h"
M2C_UNK TriggerBtn(s32);                         /* extern */

void TriggerVolbtn(s32 arg0) {
    TriggerBtn(arg0 + 0x130);
}
