#include "m2c_prelude.h"
M2C_UNK **PdcbAddLo();                          /* extern */
extern M2C_UNK DeferLoRemoveCallback;

void DeferLoRemove(void) {
    *PdcbAddLo() = &DeferLoRemoveCallback;
}
