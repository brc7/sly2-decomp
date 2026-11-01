#include "m2c_prelude.h"
M2C_UNK func_00155AE0(s32, M2C_UNK);                /* extern */
M2C_UNK StartBhv();                            /* extern */

/** @sly2-annot addr=0x155A38
 * Purpose: BHV181 slot 8: state 0
 * Info: proposed name StartBhv181 (T2); class BHV181 slot8; TU tu_1553F0
 * Notes: superseded: OnBhv181Resume (r3b_rest) | BHV vtable slot 8 role per r9 (CONFLICTS #15); was
 *   OnBhv181Resume
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r3b_rest+bhv-slot-canon(r9)
 * See: docs/reference/functions/guards-ai.md#tu_1553f0-0x1553f0-0x1564e8
 * @end-sly2-annot */
void func_00155A38(s32 arg0) {
    StartBhv();
    func_00155AE0(arg0, 0);
}
