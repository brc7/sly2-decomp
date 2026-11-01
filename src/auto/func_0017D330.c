#include "m2c_prelude.h"
M2C_UNK CloseBhv();                            /* extern */
M2C_UNK RemoveStepguardSgf(s32, M2C_UNK);                /* extern */

/** @sly2-annot addr=0x17D330
 * Purpose: BHVATTACH39 Close: lower flag 0x29
 * Info: proposed name CloseBhv188 (T2); class BHV188 slot9; TU guardbhv
 * Notes: unverified names_v2 tier-B alignment hint (tier B verified ~33% precise) | superseded:
 *   SetAloRestoreRotation (names_v2:B(unverified)) | BHV class word normalised: CloseBhvattach39 ->
 *   CloseBhv188
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources:
 *   names_v2:B(unverified)+r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_0017D330(void *arg0) {
    CloseBhv();
    RemoveStepguardSgf(M2C_FIELD(arg0, s32 *, 0x10), 0x29);
}
