#include "m2c_prelude.h"
extern s128 D_002A2DD0;
extern M2C_UNK D_002BBB60;
extern M2C_UNK D_002D9BE0;
extern M2C_UNK D_002D9C70;
extern M2C_UNK D_002D9D00;
extern s32 D_002DD5BC;
extern M2C_UNK D_002DDFA0;
extern s32 D_002E1E40;
extern s32 D_002F7900;
extern M2C_UNK D_003DD040;
extern M2C_UNK D_003DD08C;
extern M2C_UNK D_003DD490;

/** @sly2-annot addr=0x1FB2C0
 * Purpose: Set pitch (+0x18), dirty 0x40
 * Info: proposed name SetSndPitch (T1); TU sound
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_001FB2C0(s32 arg0, s32 arg1, s32 arg2, f32 fparg0) {
    if (fparg0 != M2C_FIELD(arg0, f32 *, 0x18)) {
        M2C_FIELD(arg0, f32 *, 0x18) = fparg0;
        func_001FB658(arg0, 0x40, 0x40);
    }
}
