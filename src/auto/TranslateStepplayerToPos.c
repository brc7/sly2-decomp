#include "m2c_prelude.h"
M2C_UNK TranslateStepToPos();                            /* extern */

void TranslateStepplayerToPos(void *arg0) {
    TranslateStepToPos();
    M2C_FIELD(arg0, s32 *, 0x9E4) = 0;
}
