#include "m2c_prelude.h"
M2C_UNK PvAllocSlotheapClear(s32);                         /* extern */

/** @sly2-annot addr=0x2296A0
 * Purpose: returns one XA to g_sw slotheapXa (+0x40E8)
 * Info: proposed name FreeXa (T2); TU water
 * Notes: names_v2 C proposed AddSoAngularAcceleration__FP2SOP6VECTOR (0.200)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r8_21BB88
 * See: docs/reference/functions/physics-collision.md#water-0x228b48-0x229ef0
 * @end-sly2-annot */
void func_002296A0(s32 arg0) {
    PvAllocSlotheapClear(arg0 + 0x40E8);
}
