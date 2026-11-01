#include "m2c_prelude.h"
M2C_UNK CopyAb__FPvT0Ui(s32, s32, s32);             /* extern */
M2C_UNK func_00129EC8(M2C_UNK *);                   /* extern */
s32 func_001AE318(s32 *);                           /* extern */
extern void *D_002DD2FC;
extern M2C_UNK D_002DDED0;
extern f32 D_002DDED4;
extern f32 D_002DDF10;
extern M2C_UNK D_002DDF40;
extern M2C_UNK D_003DA740;
extern M2C_UNK D_003DD490;
extern M2C_UNK D_003E11C4;

/** @sly2-annot addr=0x1E0480
 * Purpose: Splice builtin: append a RELAYCK 0x8 (SetActor) command to the relay
 *   (PrelaycmdAddRelay(prelay,0x8,fAsync)) and fill its args
 * Info: proposed name AddRelaySetActor (T1); TU relay
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void func_001E0480(s32 arg0, s32 arg1) {
    M2C_FIELD(func_001E01F8(arg0, 8, 1), s32 *, 0x10) = arg1;
}
