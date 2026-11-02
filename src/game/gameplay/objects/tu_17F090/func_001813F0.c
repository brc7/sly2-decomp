#include "m2c_prelude.h"
extern f32 D_002BE87C;

/** @sly2-annot addr=0x1813F0
 * Purpose: x384 = g * D_002BE87C
 * Info: proposed name SetHackenemyRange (T2); class HACKENEMY; TU tu_17F090
 * Notes: Cid49->Cid73 (CID 0x49=73, agent-default(hex)) | class identity from whole_game WE names/object
 *   strings: SetCid73Range -> SetHackenemyRange
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4b_rest+whole_game(class identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
void func_001813F0(void *arg0, f32 fparg0) {
    M2C_FIELD(arg0, f32 *, 0x384) = (f32) (fparg0 * D_002BE87C);
}
