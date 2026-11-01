#include "m2c_prelude.h"
M2C_UNK CloneSo();                            /* extern */

void ClonePo(void *arg0) {
    CloneSo();
    M2C_FIELD(arg0, void **, 0x168) = arg0;
}
