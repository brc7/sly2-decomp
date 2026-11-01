#include "m2c_prelude.h"
s32 PpoHero(M2C_UNK *);                       /* extern */
extern M2C_UNK D_003DD490;

s32 FIsPoCur(s32 arg0) {
    return arg0 == PpoHero(&D_003DD490);
}
