#include "m2c_prelude.h"
extern s32 D_002DD280;
extern s32 D_002DD288;
extern s32 D_002DD290;
extern s32 D_002DD298;
extern s32 D_002DD2A0;
extern s32 D_002DD2A8;
extern s32 D_002DD2B0;
extern s32 D_002DD2B8;
extern s32 D_002DD2C0;
extern s32 D_002DD2C8;
extern s32 D_002DD2D0;
extern s32 D_002DD2D8;

/** @sly2-annot addr=0x128148
 * Purpose: Resets 6 splotheap 'recyclable' cursors (D_002DD288=D_002DD280 ... stride 0x10)
 * Info: proposed name CGc_UpdateRecyclable (T1); class CGc; TU gc
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r1_116918+verdicts_tu_r1r2
 * See: docs/engine/splice-scripting.md#7-key-functions,
 *   docs/reference/functions/splice-scripting.md#gc-0x128138-0x128b60
 * @end-sly2-annot */
void CGc_UpdateRecyclable(void) {
    D_002DD288 = D_002DD280;
    D_002DD2A8 = D_002DD2A0;
    D_002DD298 = D_002DD290;
    D_002DD2B8 = D_002DD2B0;
    D_002DD2C8 = D_002DD2C0;
    D_002DD2D8 = D_002DD2D0;
}
