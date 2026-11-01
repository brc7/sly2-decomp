#include "m2c_prelude.h"
M2C_UNK func_00205910(void *, M2C_UNK *, M2C_UNK, M2C_UNK *); /* extern */
M2C_UNK func_00207350();                            /* extern */
extern M2C_UNK D_002BD910;
extern M2C_UNK D_002BD940;

/** @sly2-annot addr=0x14FAF8
 * Purpose: vt16 override of cid 0x40: base slot-16 (0x207350) then, if +0xF80 == -1 and +0x208 != 0,
 *   0x205910(this, D_002BD940, 3, D_002BD910)
 * Info: proposed name UpdateContessa (T2); class CONTESSA slot16; TU tu_14FAC8
 * Notes: class identity from whole_game WE names/object strings: UpdateCid64Guard -> UpdateContessa
 * Confidence: low (draft) [T2 0.35] - evidence: semantic review; sources: r2_131F70+whole_game(class
 *   identity)
 * See: docs/reference/functions/guards-ai.md#tu_14fac8-0x14fac8-0x14fca8
 * @end-sly2-annot */
void func_0014FAF8(void *arg0) {
    func_00207350();
    if ((M2C_FIELD(arg0, s32 *, 0xF80) == -1) && (M2C_FIELD(arg0, s32 *, 0x208) != 0)) {
        func_00205910(arg0, &D_002BD940, 3, &D_002BD910);
    }
}
