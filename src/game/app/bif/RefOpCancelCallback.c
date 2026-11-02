#include "m2c_prelude.h"
M2C_UNK func_001291A8(s32, M2C_UNK *);              /* extern */
M2C_UNK Decref__4CRef(M2C_UNK *);                   /* extern */
void SetTag__4CRef4TAGK(M2C_UNK *, M2C_UNK);          /* extern */
extern f32 D_002DDED0;
extern M2C_UNK D_002DDF40;
extern M2C_UNK D_003DD040;
extern M2C_UNK D_003DD490;
extern s32 D_003E0F9C;
extern s32 D_003E0FA0;
extern s32 D_003E0FA4;
extern M2C_UNK D_004520A8;

/** @sly2-annot addr=0x123DD8
 * Purpose: Cancels a scheduled callback by handle: 0x133130(g_sw, id) unlinks and frees; returns Void
 * Info: proposed name RefOpCancelCallback (T1); TU bif
 * Notes: splice builtin 106 cancel-callback~ -> cancel-callback~ (0.85): Body CancelSwSpliceCallback(g_sw,
 *   handle) and returns Void. Scripts store the handle returned by schedule-callback and later
 *   (cancel-callback~ s1221) (set! s122
 * Confidence: high [T1 0.85] - evidence: semantic review; sources: r1_116918+top50_round(splice builtin 106)
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
s32 RefOpCancelCallback(s32 arg0, s32 arg1, s32 arg2) {
    CancelSwSpliceCallback((s32) &D_003DD490, M2C_FIELD(arg2, s32 *, 4));
    SetTag__4CRef4TAGK((M2C_UNK *) arg0, 0x24);
    return arg0;
}
