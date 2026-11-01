#include "m2c_prelude.h"
M2C_UNK RemoveDlEntry__FP2DLPv(s32, void *);                 /* extern */
M2C_UNK func_001CE9D0(void *);                      /* extern */

void RemoveRip__FP3RIP(void *arg0) {
    RemoveDlEntry__FP2DLPv(M2C_FIELD(arg0, s32 *, 8) + 0x15C, arg0);
    func_001CE9D0(arg0);
}
