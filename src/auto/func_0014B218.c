#include "m2c_prelude.h"
M2C_UNK func_00154038(s32, s32, M2C_UNK);           /* extern */
M2C_UNK func_002075D0();                            /* extern */
s32 func_00209A68(s32, M2C_UNK);                    /* extern */

/** @sly2-annot addr=0x14B218
 * Purpose: vt5: STEPGUARD handler; msg 0xB about itself when flag 0x24 clear -> 0x154038
 * Info: proposed name HandleCarmelitaMessage (T2); class CARMELITA slot5; TU tu_14A008
 * Notes: Cid3F->Cid63 (CID 0x3F=63, hex-literal) | class identity from whole_game WE names/object strings:
 *   HandleCid63Message -> HandleCarmelitaMessage
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r2b_rest+whole_game(class identity)
 * See: docs/reference/functions/guards-ai.md#tu_14a008-0x14a008-0x14b660
 * @end-sly2-annot */
void func_0014B218(s32 arg0, s32 arg1, void *arg2) {
    void *temp_v0;

    func_002075D0();
    if ((func_00209A68(arg0, 0x24) == 0) && (arg1 == 0xB)) {
        temp_v0 = M2C_FIELD(arg2, void **, 0);
        if (temp_v0 == arg0) {
            func_00154038(M2C_FIELD(temp_v0, s32 *, 0xF64), M2C_FIELD(arg2, s32 *, 4), 1);
        }
    }
}
