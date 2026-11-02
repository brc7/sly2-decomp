#include "m2c_prelude.h"
M2C_UNK ShowLetterbox(M2C_UNK *);                   /* extern */
extern M2C_UNK D_002E1E60;

/** @sly2-annot addr=0x20F128
 * Purpose: ++SW+0x3D80; first -> show letterbox object (0x1A3078(&D_002E1E60): hides subtitles, vt49)
 * Info: proposed name AcquireSwLetterbox (T1); TU sw
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/engine/world-sw.md#5-hands-off-and-locks,
 *   docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void AcquireSwLetterbox(void *arg0) {
    s32 temp_v0;

    temp_v0 = M2C_FIELD(arg0, s32 *, 0x3D80) + 1;
    M2C_FIELD(arg0, s32 *, 0x3D80) = temp_v0;
    if (temp_v0 == 1) {
        ShowLetterbox(&D_002E1E60);
    }
}
