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

/** @sly2-annot addr=0x1E1940
 * Purpose: Splice builtin: append a RELAYCK 0x52 (Unk52) command to the relay
 *   (PrelaycmdAddRelay(prelay,0x52,fAsync)) and fill its args
 * Info: proposed name AddRelayUnk52 (T2); TU relay
 * Notes: v2: demoted to T2 (placeholder token in name: Unk)
 * Confidence: low (draft) [T2 0.50] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void func_001E1940(s32 arg0, f32 fparg0) {
    M2C_FIELD(PrelaycmdAddRelay(arg0, 0x52, 0), f32 *, 0x10) = fparg0;
}
