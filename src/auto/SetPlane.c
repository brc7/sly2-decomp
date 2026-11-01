#include "m2c_prelude.h"
void SetPlane(void *arg0, s128 arg1, f32 fparg0) {
    M2C_FIELD(arg0, s128 *, 0) = arg1;
    M2C_FIELD(arg0, f32 *, 0xC) = fparg0;
}
