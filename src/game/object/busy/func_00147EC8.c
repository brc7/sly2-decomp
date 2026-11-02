#include "m2c_prelude.h"
M2C_UNK RemoveDlEntry__FP2DLPv(s32);                         /* extern */

/** @sly2-annot addr=0x147EC8
 * Purpose: RemoveDlEntry(psw+0x20, x)
 * Info: proposed name RemoveSwBusyEntry (T2); TU busy
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/object-model.md#busy-0x147e20-0x148620
 * @end-sly2-annot */
void func_00147EC8(s32 arg0) {
    RemoveDlEntry__FP2DLPv(arg0 + 0x20);
}
