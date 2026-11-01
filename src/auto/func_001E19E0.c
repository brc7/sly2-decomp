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

/** @sly2-annot addr=0x1E19E0
 * Purpose: Splice builtin: append a RELAYCK 0x55 (SetSsgGoal) command to the relay
 *   (PrelaycmdAddRelay(prelay,0x55,fAsync)) and fill its args
 * Info: proposed name AddRelaySetSsgGoal (T1); TU relay
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void func_001E19E0(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    void *temp_v0;

    temp_v0 = func_001E01F8(arg0, 0x55, arg3);
    M2C_FIELD(temp_v0, s32 *, 0x10) = arg1;
    M2C_FIELD(temp_v0, s32 *, 0x14) = arg2;
}
