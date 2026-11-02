#include "m2c_prelude.h"
M2C_UNK AddLightToSw(M2C_UNK *, s32);              /* extern */
M2C_UNK OnLoAdd();                            /* extern */
extern M2C_UNK D_003DD490;

/** @sly2-annot addr=0x1A3518
 * Purpose: LIGHT vt23: LO Unfreeze then AddLightToSw (alloc slot, insert into light tree)
 * Info: proposed name OnLightAdd (T1); class LIGHT slot23; TU light
 * Notes: names_v2 v1 proposed RemoveLightFromSw__FP5LIGHT (0.004) | slot 23 OnAdd resolved by arbitration
 *   (OnRemove/OnAdd = object leaves/enters the active world, incl. proximity activation) (was LIGHT_Unfreeze)
 * Confidence: high [T1 0.85] - evidence: arbitration; static naming pass; sources: arbitration_slots22_23
 * See: docs/engine/rendering.md#13-key-functions,
 *   docs/reference/functions/rendering.md#light-0x1a3260-0x1a4e08
 * @end-sly2-annot */
void OnLightAdd(s32 arg0) {
    OnLoAdd();
    AddLightToSw(&D_003DD490, arg0);
}
