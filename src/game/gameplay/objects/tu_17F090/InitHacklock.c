#include "m2c_prelude.h"
M2C_UNK func_0017F090();                            /* extern */
M2C_UNK func_0017F3E0(s32, M2C_UNK, s32);           /* extern */

/** @sly2-annot addr=0x181408
 * Purpose: CID 0x4B vt7: InitCid47 + 3 entries
 * Info: proposed name InitHacklock (T1); class HACKLOCK slot7; TU tu_17F090
 * Notes: Cid4B->Cid75 (CID 0x4B=75, hex-literal) | class identity from whole_game WE names/object strings:
 *   InitCid75 -> InitHacklock
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r4b_rest+whole_game(class identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
void InitHacklock(s32 arg0) {
    func_0017F090();
    func_0017F3E0(arg0, 3, arg0 + 0x1D0);
}
