#include "m2c_prelude.h"
M2C_UNK CloneSo();                            /* extern */

void CloneButton(void *arg0) {
    CloneSo();
    M2C_FIELD(arg0, void **, 0x150) = arg0;
}
