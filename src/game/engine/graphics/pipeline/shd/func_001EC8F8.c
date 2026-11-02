#include "m2c_prelude.h"
extern s32 D_003DCFEC;

/** @sly2-annot addr=0x1EC8F8
 * Purpose: &D_003DCFEC[i] (stride 0x2C)
 * Info: proposed name PshdFromIshd (T1); TU shd
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r6b_rest+verdicts_followups
 * See: docs/engine/rendering.md#13-key-functions,
 *   docs/formats/textures.md#4-shd-record-shader-and-tex-cstatic,
 *   docs/reference/functions/rendering.md#shd-0x1ea568-0x1ed7a8
 * @end-sly2-annot */
s32 func_001EC8F8(s32 arg0) {
    return D_003DCFEC + (arg0 * 0x2C);
}
