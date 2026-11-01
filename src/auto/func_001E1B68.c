#include "m2c_prelude.h"
M2C_UNK func_00117968(s32, s32, s32);               /* extern */
M2C_UNK func_00117EB0(s32, s32);                    /* extern */
extern s128 D_002A2DD0;
extern f32 D_002A8124;
extern f32 D_002A8130;
extern f32 D_002A8134;
extern M2C_UNK D_002D8C08;
extern f32 D_002DDED0;
extern M2C_UNK D_002DDF40;
extern M2C_UNK D_002DDFA0;
extern M2C_UNK D_002DE440;
extern M2C_UNK D_003DD490;
extern M2C_UNK D_003E1560;

/** @sly2-annot addr=0x1E1B68
 * Purpose: --cref; at 0 release sound ref, destruct and free to the JSG slot heap D_003E1560
 * Info: proposed name ReleaseJsg (T1); class JSG; TU relay
 * Notes: agent qualifier: (DecrefJsg)
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r6b_rest
 * See: docs/engine/cutscenes.md#5-key-functions,
 *   docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void func_001E1B68(s32 arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = M2C_FIELD(arg0, s32 *, 0x10) - 1;
    M2C_FIELD(arg0, s32 *, 0x10) = temp_v0;
    if (temp_v0 <= 0) {
        func_00256EE0(arg0, 0);
        func_002580A8(arg0 + 4, 0);
        UnsubscribeLoAll(arg0);
        func_0011CD98((s32) &D_003E1560, arg0);
    }
}
