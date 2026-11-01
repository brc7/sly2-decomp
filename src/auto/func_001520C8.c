#include "m2c_prelude.h"
M2C_UNK func_001524A0(s32, M2C_UNK, M2C_UNK);       /* extern */
M2C_UNK func_001F2D48();                            /* extern */

/** @sly2-annot addr=0x1520C8
 * Purpose: cid 0x44 vt12 override: base (0x1F2D48) then clear its animation (ApplyDancerAseg(this,0,0))
 * Info: proposed name RemoveHierarchyDancer (T2); class DANCER slot12; TU rhythm
 * Confidence: low (draft) [T2 0.35] - evidence: semantic review; sources: r2_131F70
 * See: docs/engine/camera.md#4-source-files, docs/engine/gameplay-objects.md#gameplay-object-classes,
 *   docs/engine/gameplay-objects.md#source-files
 * @end-sly2-annot */
void func_001520C8(s32 arg0) {
    func_001F2D48();
    func_001524A0(arg0, 0, 0);
}
