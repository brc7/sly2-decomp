#include "m2c_prelude.h"
M2C_UNK AddSwAaobrObject(M2C_UNK *, s32);              /* extern */
extern M2C_UNK D_003DD490;

/** @sly2-annot addr=0x1F56F8
 * Purpose: SO vt53: create tree record via g_sw (0x1336F0)
 * Info: proposed name SO_PtreeNew (T2); class SO slot53; TU so
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F56F8(s32 arg0) {
    AddSwAaobrObject(&D_003DD490, arg0);
}
