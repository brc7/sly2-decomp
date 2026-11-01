#include "m2c_prelude.h"
extern M2C_UNK D_002BE870;

/** @sly2-annot addr=0x1813C0
 * Purpose: Scale default vector D_002BE870 by g into x378..
 * Info: proposed name SetHackenemyScale (T2); class HACKENEMY; TU tu_17F090
 * Notes: names_v2 v1 proposed InitSgg__FP3SGG (0.002) | Cid49->Cid73 (CID 0x49=73, agent-default(hex)) |
 *   class identity from whole_game WE names/object strings: SetCid73Scale -> SetHackenemyScale
 * Confidence: low (draft) [T2 0.30] - evidence: static naming pass; sources: r4b_rest+whole_game(class
 *   identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
void func_001813C0(void *arg0, f32 fparg0) {
    M2C_FIELD(arg0, f32 *, 0x378) = (f32) (M2C_FIELD(&D_002BE870, f32 *, 0) * fparg0);
    M2C_FIELD(arg0, f32 *, 0x37C) = (f32) (M2C_FIELD(&D_002BE870, f32 *, 4) * fparg0);
    M2C_FIELD(arg0, f32 *, 0x380) = (f32) M2C_FIELD(&D_002BE870, f32 *, 8);
}
