#include "m2c_prelude.h"
M2C_UNK func_00168638(void *, M2C_UNK, s32, f32, f32); /* extern */
M2C_UNK func_001F3C10(void *);                      /* extern */
extern s32 D_002DDF54;

/** @sly2-annot addr=0x1F3C70
 * Purpose: SO vt19: if visible in current vis mask (+0x2D & D_002DDF54) draw globset (+0x80) with alpha
 *   +0x12C
 * Info: proposed name RenderSoSelf (T1); class SO slot19; TU so
 * Notes: names_v2 C proposed SetSoNoXpsCenter__FP2SOi (0.438)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r6_1D5150+verdicts_tu_r4r6
 * See: docs/engine/physics-collision.md#7-key-functions, docs/engine/rendering.md#1-frame-structure,
 *   docs/reference/functions/particles-effects.md#trail-0x21bb88-0x21c820
 * @end-sly2-annot */
void func_001F3C70(void *arg0) {
    f32 temp_f12;

    if (M2C_FIELD(arg0, u8 *, 0x2D) & D_002DDF54) {
        func_001F3C10(arg0);
        temp_f12 = M2C_FIELD(arg0, f32 *, 0x12C);
        func_00168638(arg0 + 0x80, 0, M2C_FIELD(arg0, s32 *, 0x134), temp_f12, temp_f12);
    }
}
