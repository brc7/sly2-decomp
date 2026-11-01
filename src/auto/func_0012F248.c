#include "m2c_prelude.h"
M2C_UNK func_0011A1F8(s32, M2C_UNK);                /* extern */
M2C_UNK func_001A5270();                            /* extern */

/** @sly2-annot addr=0x12F248
 * Purpose: InitLo (0x1A5270) then InitDl of the ASEGA list; two speed/weight fields = 1.0
 * Info: proposed name InitAseg (T1); class ASEG slot7; TU aseg
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r1_116918
 * See: docs/engine/animation.md#1-aseg-and-asega-asegcpp-0x12f248-asegacpp-0x1308a8,
 *   docs/engine/animation.md#6-source-files, docs/engine/animation.md#7-key-functions
 * @end-sly2-annot */
void func_0012F248(void *arg0) {
    void *temp_v0;

    func_001A5270();
    func_0011A1F8(arg0 + 0x50, 0x2C);
    temp_v0 = arg0 + 0x80;
    M2C_FIELD(arg0, f32 *, 0x78) = 1.0f;
    M2C_FIELD(arg0, f32 *, 0x80) = 1.0f;
    M2C_FIELD(temp_v0, s32 *, 4) = 0;
    M2C_FIELD(temp_v0, s32 *, 8) = 0;
    M2C_FIELD(arg0, f32 *, 0x94) = 1.0f;
    M2C_FIELD(arg0, s32 *, 0x90) = 0;
    M2C_FIELD(arg0, s32 *, 0x40) = (s32) (M2C_FIELD(arg0, s32 *, 0x40) | 0x40);
}
