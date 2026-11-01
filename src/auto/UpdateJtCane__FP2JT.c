#include "m2c_prelude.h"
M2C_UNK UpdateJtHands(void *);                      /* extern */

void UpdateJtCane__FP2JT(void *arg0) {
    if (M2C_FIELD(arg0, s32 *, 0x18D8) == 0) {
        UpdateJtHands(arg0 + 0x17D0);
    }
}
