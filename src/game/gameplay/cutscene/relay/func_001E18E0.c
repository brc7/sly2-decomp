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

/** @sly2-annot addr=0x1E18E0
 * Purpose: Splice builtin: append a RELAYCK 0x50 (Unk50) command to the relay
 *   (PrelaycmdAddRelay(prelay,0x50,fAsync)) and fill its args
 * Info: proposed name AddRelayUnk50 (T2); TU relay
 * Notes: v2: demoted to T2 (placeholder token in name: Unk)
 * Confidence: low (draft) [T2 0.50] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void func_001E18E0(s32 arg0, s32 arg1) {
    M2C_FIELD(func_001E01F8(arg0, 0x50, 0), s32 *, 0x10) = arg1;
}
