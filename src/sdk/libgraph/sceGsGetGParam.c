#include "m2c_prelude.h"
extern M2C_UNK D_002C66F0;

// Returns &gs param block (D_002C66F0). (libgraph graph001.o) SDK name sceGsGetGParam.
M2C_UNK *sceGsGetGParam(void) {
    return &D_002C66F0;
}
