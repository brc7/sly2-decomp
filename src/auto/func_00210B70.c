#include "m2c_prelude.h"
s32 func_00210AE0();                                /* extern */
M2C_UNK func_00210B98(s32);                         /* extern */

/** @sly2-annot addr=0x210B70
 * Purpose: PrecWorldEntry(NWorldCur())
 * Info: proposed name PrecWorldEntryCur (T2); TU sw
 * Notes: names_v2 v1 proposed SetVolbtnButtons__FP6VOLBTN7BUTTONS (0.000)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r7_1F8DD8
 * See: docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void func_00210B70(void) {
    func_00210B98(func_00210AE0());
}
