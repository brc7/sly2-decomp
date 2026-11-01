#include "m2c_prelude.h"
/** @sly2-annot addr=0x19C8B0
 * Purpose: If x28 set, *pgrf
 * Info: proposed name AddJtAirCpdefiFlags (T2); TU air
 * Notes: names_v2 C proposed ResetJmtList__Fv (0.170)
 * Confidence: low (draft) [T2 0.30] - evidence: static naming pass; sources: r4b_rest
 * See: docs/reference/functions/player.md#air-0x195950-0x19cbf8
 * @end-sly2-annot */
void func_0019C8B0(void *arg0, s32 *arg1) {
    if (M2C_FIELD(arg0, s32 *, 0x28) != 0) {
        *arg1 |= 1;
    }
}
