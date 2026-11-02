#include "m2c_prelude.h"
M2C_UNK func_001A3E88(M2C_UNK *, s32);              /* extern */
M2C_UNK OnLoRemove();                            /* extern */
extern M2C_UNK D_003DD490;

/** @sly2-annot addr=0x1A34E0
 * Purpose: LIGHT vt22: LO Freeze then RemoveLightFromSw (free tree slot)
 * Info: proposed name OnLightRemove (T1); class LIGHT slot22; TU light
 * Notes: names_v2 v1 proposed UpdateLightXfWorldHierarchy__FP5LIGHT (0.001) | slot 22 OnRemove resolved by
 *   arbitration (OnRemove/OnAdd = object leaves/enters the active world, incl. proximity activation) (was
 *   LIGHT_Freeze)
 * Confidence: high [T1 0.85] - evidence: arbitration; static naming pass; sources: arbitration_slots22_23
 * See: docs/engine/rendering.md#13-key-functions,
 *   docs/reference/functions/rendering.md#light-0x1a3260-0x1a4e08
 * @end-sly2-annot */
void func_001A34E0(s32 arg0) {
    OnLoRemove();
    func_001A3E88(&D_003DD490, arg0);
}
