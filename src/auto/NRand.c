#include "m2c_prelude.h"
s32 rand();                                         /* extern */

s32 NRand(void) {
    return rand() % 905010707;
}
