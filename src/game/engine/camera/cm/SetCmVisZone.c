#include "m2c_prelude.h"
/** @sly2-annot addr=0x14E148
 * Purpose: If changed: pzonVis(+0x10)=pzon, grfvis(+0x14)=pzon->+0xE0
 * Info: proposed name SetCmVisZone (T1); TU cm
 * Notes: names_v2 v1 proposed SetCmUFogMax__FP2CMf (0.001)
 * Confidence: mid (draft) [T1 0.65] - evidence: semantic review; static naming pass; sources: r2_131F70
 * See: docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
void SetCmVisZone(void *arg0, void *arg1) {
    if (arg1 != M2C_FIELD(arg0, void **, 0x10)) {
        M2C_FIELD(arg0, void **, 0x10) = arg1;
        M2C_FIELD(arg0, s32 *, 0x14) = (s32) M2C_FIELD(arg1, s32 *, 0xE0);
    }
}
