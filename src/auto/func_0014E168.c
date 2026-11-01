#include "m2c_prelude.h"
M2C_UNK func_0014DC58(void *);                      /* extern */

/** @sly2-annot addr=0x14E168
 * Purpose: Copies a 4x4 placement into CM::mat (+0x60..0x9F) then UpdateCmMat4
 * Info: proposed name SetCmMat4 (T1); TU cm
 * Notes: names_v2 B proposed ResetCmAdjust__FP2CMP2SOP6CPDEFI (0.856)
 * Confidence: mid (draft) [T1 0.75] - evidence: semantic review; static naming pass; sources: r2_131F70
 * See: docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
void func_0014E168(void *arg0, void *arg1) {
    M2C_FIELD(arg0, s128 *, 0x60) = (s128) M2C_FIELD(arg1, s128 *, 0);
    M2C_FIELD(arg0, s128 *, 0x70) = (s128) M2C_FIELD(arg1, s128 *, 0x10);
    M2C_FIELD(arg0, s128 *, 0x80) = (s128) M2C_FIELD(arg1, s128 *, 0x20);
    M2C_FIELD(arg0, s128 *, 0x90) = (s128) M2C_FIELD(arg1, s128 *, 0x30);
    func_0014DC58(arg0);
}
