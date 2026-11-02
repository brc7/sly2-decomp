#include "m2c_prelude.h"
extern s32 D_003DCFE8;
extern s32 D_003DCFEC;
extern s32 D_003DCFF0;
extern s32 D_003DCFF4;
extern s32 D_003DCFF8;
extern s32 D_003DCFFC;
extern s32 D_003DD000;
extern s32 D_003DD004;

/** @sly2-annot addr=0x1EC7A0
 * Purpose: Zero shader/bitmap/clut counts and arrays (D_003DCFE8..D_003DD004)
 * Info: proposed name ResetShaders (T1); TU shd
 * Notes: textures format doc cites this function by this name (CONFIRMED(code) in the doc)
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r6b_rest+format-doc:textures
 * See: docs/engine/rendering.md#9-textures-cluts-and-gs-memory-shdcpp-0x1ea568-gscpp-0x16f2d0-cstatic,
 *   docs/formats/textures.md#6-runtime-notes, docs/reference/functions/rendering.md#shd-0x1ea568-0x1ed7a8
 * @end-sly2-annot */
void ResetShaders(void) {
    D_003DCFE8 = 0;
    D_003DCFEC = 0;
    D_003DCFF0 = 0;
    D_003DCFF4 = 0;
    D_003DCFF8 = 0;
    D_003DCFFC = 0;
    D_003DD000 = 0;
    D_003DD004 = 0;
}
