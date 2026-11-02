#include "m2c_prelude.h"
M2C_UNK ColorCorrectRgba(u32);                         /* extern */
extern s32 D_003DCFF0;
extern void *D_003DCFF4;
extern s32 D_003DCFF8;
extern s32 D_003DCFFC;

/** @sly2-annot addr=0x1EA568
 * Purpose: After shared-data load: for each color table/bitmap record flagged shared (b4==1) add the load
 *   offset to its data pointer and color-correct its entries
 * Info: proposed name RelocateSharedColorTables (T1); TU shd
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels |
 *   textures format doc cites this function by this name (CONFIRMED(code) in the doc)
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources:
 *   r6b_rest+parse-verified+format-doc:textures
 * See: docs/engine/physics-collision.md#6-source-files, docs/engine/rendering.md#12-source-files,
 *   docs/engine/rendering.md#13-key-functions
 * @end-sly2-annot */
void RelocateSharedColorTables(s32 arg0) {
    s32 var_a0;
    s32 var_a1;
    u32 temp_v1;
    u32 var_s0;
    void *temp_s1;
    void *var_v1;

    var_a1 = 0;
    if (D_003DCFF8 > 0) {
        do {
            temp_s1 = D_003DCFFC + (var_a1 << 5);
            if (M2C_FIELD(temp_s1, u8 *, 4) == 1) {
                temp_v1 = arg0 + M2C_FIELD(temp_s1, u32 *, 0xC);
                M2C_FIELD(temp_s1, u32 *, 0xC) = temp_v1;
                var_s0 = temp_v1;
                if (var_s0 < (u32) (var_s0 + (M2C_FIELD(temp_s1, u16 *, 8) * 4))) {
                    do {
                        ColorCorrectRgba(var_s0);
                        var_s0 += 4;
                    } while (var_s0 < (u32) (M2C_FIELD(temp_s1, u32 *, 0xC) + (M2C_FIELD(temp_s1, u16 *, 8) * 4)));
                }
            }
            var_a1 += 1;
        } while (var_a1 < D_003DCFF8);
    }
    if (D_003DCFF0 > 0) {
        var_a0 = D_003DCFF0;
        var_v1 = D_003DCFF4;
        do {
            var_a0 -= 1;
            if (M2C_FIELD(var_v1, u8 *, 4) == 1) {
                M2C_FIELD(var_v1, s32 *, 0x14) = (s32) (arg0 + M2C_FIELD(var_v1, s32 *, 0x14));
            }
            var_v1 += 0x28;
        } while (var_a0 != 0);
    }
}
