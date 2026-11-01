#include "m2c_prelude.h"
M2C_UNK CopyAb__FPvT0Ui(s32, s32, s32);             /* extern */
void **func_001AE318(s32 *);                        /* extern */
extern s128 D_002A2DD0;
extern M2C_UNK D_002C3C20;
extern M2C_UNK D_002C44F0;
extern void *D_002DD2FC;
extern f32 D_002DDED0;
extern f32 D_002DDED4;
extern M2C_UNK D_003DD490;
extern void *D_003E1218;
extern void *D_003E126C;
extern M2C_UNK D_003E15A8;
extern s32 *D_003E15C0;

/** @sly2-annot addr=0x1F7588
 * Purpose: return arg1->+0x44 != 0
 * Info: proposed name FZprHasField44 (T2); class SO slot75; TU so
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: r6_1D5150
 * See: docs/engine/physics-collision.md#1-so-the-solid-object-socpp-0x1f2108-0x1f8dd8-174-functions,
 *   docs/engine/player.md#2-po-and-poctl-pocpp-0x1c3d70,
 *   docs/reference/functions/gameplay-objects.md#tu_1d1370-0x1d1370-0x1d5150
 * @end-sly2-annot */
s32 func_001F7588(s32 arg0, s32 arg1) {
    return M2C_FIELD(arg1, s32 *, 0x44) != 0;
}
