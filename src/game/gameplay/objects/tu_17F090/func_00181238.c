#include "m2c_prelude.h"
M2C_UNK func_0017F6B0();                            /* extern */
M2C_UNK func_00181268(s32, M2C_UNK);                /* extern */

/** @sly2-annot addr=0x181238
 * Purpose: Reset health and state
 * Info: proposed name ResetHackenemy (T2); class HACKENEMY slot83; TU tu_17F090
 * Notes: names_v2 v1 proposed MatchStepguardAnimationPhase__FP9STEPGUARD3OIDN31 (0.638) | Cid49->Cid73 (CID
 *   0x49=73, vtable) | class identity from whole_game WE names/object strings: ResetCid73 -> ResetHackenemy
 * Confidence: low (draft) [T2 0.30] - evidence: static naming pass; sources: r4b_rest+whole_game(class
 *   identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
void func_00181238(s32 arg0) {
    func_0017F6B0();
    func_00181268(arg0, 0);
}
