#include "m2c_prelude.h"
M2C_UNK CopyAb__FPvT0Ui(s32, s32, s32);             /* extern */
M2C_UNK Decref__4CRef(M2C_UNK *);                   /* extern */
s32 func_001AE318(s32 *);                           /* extern */
extern void *D_002DD2FC;
extern M2C_UNK D_002DDED0;
extern f32 D_002DDED4;
extern f32 D_002DDF10;
extern M2C_UNK D_002DDF40;
extern M2C_UNK D_003DA740;
extern M2C_UNK D_003DD490;
extern M2C_UNK D_003E11C4;

/** @sly2-annot addr=0x1E0610
 * Purpose: Splice builtin: append a RELAYCK 0xE (WidgetShow) command to the relay
 *   (PrelaycmdAddRelay(prelay,0xE,fAsync)) and fill its args
 * Info: proposed name AddRelayWidgetShow (T1); TU relay
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void AddRelayWidgetShow(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    void *temp_v0;

    temp_v0 = PrelaycmdAddRelay(arg0, 0xE, arg3);
    M2C_FIELD(temp_v0, s32 *, 0x10) = arg1;
    M2C_FIELD(temp_v0, s32 *, 0x14) = arg2;
}
