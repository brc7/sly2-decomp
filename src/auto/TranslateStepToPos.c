#include "m2c_prelude.h"
s128 ConvertAloPos(void *, M2C_UNK, s128);          /* extern */
M2C_UNK TranslateSoToPos();                            /* extern */

void TranslateStepToPos(void *arg0) {
    TranslateSoToPos();
    M2C_FIELD(arg0, s128 *, 0x230) = ConvertAloPos(arg0, 0, M2C_FIELD(arg0, s128 *, 0x240));
}
