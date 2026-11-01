#include "m2c_prelude.h"
void EnableStuckWatch(void *arg0) {
    M2C_FIELD(arg0, s32 *, 0xC) = 1;
}
