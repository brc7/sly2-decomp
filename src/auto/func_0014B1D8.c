#include "m2c_prelude.h"
s32 func_0013E3C8(M2C_UNK, M2C_UNK, M2C_UNK, M2C_UNK); /* extern */
M2C_UNK func_002061D8();                            /* extern */

/** @sly2-annot addr=0x14B1D8
 * Purpose: vt15: STEPGUARD post-load, create helper object PloNew(3,0,0x62B,-1) at +0x1010
 * Info: proposed name PostCarmelitaLoad (T2); class CARMELITA slot15; TU tu_14A008
 * Notes: Cid3F->Cid63 (CID 0x3F=63, hex-literal) | verifier PLAUSIBLE: Placeholder class name. CID 0x3F
 *   (STEPGUARD subclass) vtable slot 15 PostLoad: PostStepguardLoad then +0x1010 = PloNew(3,0,0x62B,-1). |
 *   class identity from whole_game WE names/object strings: PostCid63Load -> PostCarmelitaLoad | T2 forced:
 *   verifier: placeholder name
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources:
 *   r2b_rest+verdicts_followups+whole_game(class identity)
 * See: docs/reference/functions/guards-ai.md#tu_14a008-0x14a008-0x14b660
 * @end-sly2-annot */
void func_0014B1D8(void *arg0) {
    func_002061D8();
    M2C_FIELD(arg0, s32 *, 0x1010) = func_0013E3C8(3, 0, 0x62B, -1);
}
