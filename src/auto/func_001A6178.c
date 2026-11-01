#include "m2c_prelude.h"
M2C_UNK func_00210140(M2C_UNK *, s32);              /* extern */
extern M2C_UNK D_003DD490;

/** @sly2-annot addr=0x1A6178
 * Purpose: LO vt23: register in g_sw update table (0x210140, compacts when >10% dead)
 * Info: proposed name OnLoAdd (T1); class LO slot23; TU lo
 * Notes: slot 23 OnAdd resolved by arbitration (OnRemove/OnAdd = object leaves/enters the active world, incl.
 *   proximity activation) (was LO_Unfreeze) | runtime: update-list membership consistent with OnAdd/OnRemove
 *   (RAM cannot discriminate)
 * Confidence: high [T1 0.85] - evidence: runtime (save-state RAM); arbitration; sources:
 *   arbitration_slots22_23
 * See: docs/engine/gameplay-objects.md#key-functions, docs/engine/object-model.md#2-the-lo-virtual-interface,
 *   docs/engine/object-model.md#9-key-functions
 * @end-sly2-annot */
void func_001A6178(s32 arg0) {
    func_00210140(&D_003DD490, arg0);
}
