#include "m2c_prelude.h"
M2C_UNK func_0017F138();                            /* extern */
M2C_UNK func_0017F408(s32, M2C_UNK, s32);           /* extern */

/** @sly2-annot addr=0x181440
 * Purpose: vt15: PostCid47Load + bind entries
 * Info: proposed name PostHacklockLoad (T2); class HACKLOCK slot15; TU tu_17F090
 * Notes: Cid4B->Cid75 (CID 0x4B=75, hex-literal) | class identity from whole_game WE names/object strings:
 *   PostCid75Load -> PostHacklockLoad
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r4b_rest+whole_game(class identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
void func_00181440(s32 arg0) {
    func_0017F138();
    func_0017F408(arg0, 3, arg0 + 0x1D0);
}
