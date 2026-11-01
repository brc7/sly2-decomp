#include "m2c_prelude.h"
M2C_UNK CloneXfm();                            /* extern */

void CloneVolbtn(void *arg0) {
    CloneXfm();
    M2C_FIELD(arg0, void **, 0x130) = arg0;
}
