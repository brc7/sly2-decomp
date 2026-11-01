#include "m2c_prelude.h"
s32 PcgtPointInCbspQuick(s32);                             /* extern */

s32 FPosInPathzone(s32 arg0) {
    return PcgtPointInCbspQuick(arg0 + 0xE0) != 0;
}
