#include "m2c_prelude.h"
M2C_UNK func_0017F6B0();                            /* extern */
M2C_UNK func_0017FD08(s32, M2C_UNK);                /* extern */
M2C_UNK UpdateHackshipHealthBar(s32);                         /* extern */
s32 FIsPoActive(s32);                             /* extern */

/** @sly2-annot addr=0x17FCB0
 * Purpose: Reset health and state (ResetCid47 + SetCid48State + health bar)
 * Info: proposed name ResetHackship (T2); class HACKSHIP slot83; TU tu_17F090
 * Notes: names_v2 v1 proposed FUN_001ca6d0 (0.000) | Cid48->Cid72 (CID 0x48=72, vtable) | class identity from
 *   whole_game WE names/object strings: ResetCid72 -> ResetHackship
 * Confidence: low (draft) [T2 0.30] - evidence: static naming pass; sources: r4b_rest+whole_game(class
 *   identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
void func_0017FCB0(s32 arg0) {
    func_0017F6B0();
    if (FIsPoActive(arg0) != 0) {
        func_0017FD08(arg0, 1);
    } else {
        func_0017FD08(arg0, 0);
    }
    UpdateHackshipHealthBar(arg0);
}
