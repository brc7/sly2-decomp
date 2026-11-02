#include "m2c_prelude.h"
M2C_UNK RemoveDlEntry__FP2DLPv(M2C_UNK *, s32);              /* extern */
M2C_UNK OnLoRemove();                            /* extern */
extern M2C_UNK D_003E1438;

/** @sly2-annot addr=0x1844D8
 * Purpose: vt22: Freeze + remove from D_003E1438
 * Info: proposed name OnCid27Remove (T2); class CID27 slot22; TU grabspot
 * Notes: names_v2 B proposed FIgnoreHbskIntersection__FP4HBSKP2SO (0.856) | slot 22 OnRemove resolved by
 *   arbitration (OnRemove/OnAdd = object leaves/enters the active world, incl. proximity activation) (was
 *   CID27_Freeze)
 * Confidence: low (draft) [T2 0.85] - evidence: arbitration; static naming pass; sources:
 *   arbitration_slots22_23
 * See: docs/engine/player.md#11-key-functions, docs/reference/functions/player.md#grabspot-0x182c70-0x1848a8
 * @end-sly2-annot */
void func_001844D8(s32 arg0) {
    OnLoRemove();
    RemoveDlEntry__FP2DLPv(&D_003E1438, arg0);
}
