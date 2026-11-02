#include "m2c_prelude.h"
M2C_UNK func_001A30D0(M2C_UNK *);                   /* extern */
extern M2C_UNK D_002E1E60;

/** @sly2-annot addr=0x20F160
 * Purpose: --SW+0x3D80; last -> 0x1A30D0 (vt50 hide)
 * Info: proposed name ReleaseSwLetterbox (T1); TU sw
 * Notes: names_v2 C proposed IsSwVagPlaying__FP2SWPi (0.438)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r7_1F8DD8+verdicts_tu_r7r8
 * See: docs/engine/world-sw.md#10-key-functions, docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void ReleaseSwLetterbox(void *arg0) {
    s32 temp_v0;

    temp_v0 = M2C_FIELD(arg0, s32 *, 0x3D80) - 1;
    M2C_FIELD(arg0, s32 *, 0x3D80) = temp_v0;
    if (temp_v0 == 0) {
        func_001A30D0(&D_002E1E60);
    }
}
