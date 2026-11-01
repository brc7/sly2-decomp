#include "m2c_prelude.h"
s32 PloNew__F3CIDP2SO3OIDi(M2C_UNK, M2C_UNK, M2C_UNK, M2C_UNK); /* extern */
extern s32 D_003DD48C;

void EnsureStepguardManager(void) {
    if (D_003DD48C == 0) {
        D_003DD48C = PloNew__F3CIDP2SO3OIDi(0x7D, 0, 0x656, -1);
    }
}
