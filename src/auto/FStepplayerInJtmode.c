#include "m2c_prelude.h"
s32 FStepplayerInJtmode(void *arg0) {
    return (u32) M2C_FIELD(arg0, u32 *, 0xF04) < 3U;
}
