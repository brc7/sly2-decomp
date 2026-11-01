#include "m2c_prelude.h"
M2C_UNK func_001A53A0(void *);                      /* extern */
M2C_UNK func_001A53E0();                            /* extern */

/** @sly2-annot addr=0x1A5B80
 * Purpose: LO vt13 (plo, ploOld?, f): with parent -> share parent loroot (free own if it was a root); no
 *   parent and f -> EnsureLoroot
 * Info: proposed name UpdateLoroot (T1); class LO slot13; TU lo
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r4_17F090
 * See: docs/engine/object-model.md#2-the-lo-virtual-interface,
 *   docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518
 * @end-sly2-annot */
void func_001A5B80(void *arg0, s32 arg1, s32 arg2) {
    if (M2C_FIELD(arg0, void **, 0x10) != NULL) {
        if ((arg1 == arg0) && (arg2 == 0)) {
            func_001A53E0();
        }
        M2C_FIELD(arg0, s32 *, 0x14) = (s32) M2C_FIELD(M2C_FIELD(arg0, void **, 0x10), s32 *, 0x14);
        return;
    }
    if (arg2 != 0) {
        M2C_FIELD(arg0, s32 *, 0x14) = 0;
        func_001A53A0(arg0);
    }
}
