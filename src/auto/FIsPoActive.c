#include "m2c_prelude.h"
s32 PpoCur(M2C_UNK *);                       /* extern */
extern M2C_UNK D_003DD490;

s32 FIsPoActive(s32 arg0) {
    return arg0 == PpoCur(&D_003DD490);
}
