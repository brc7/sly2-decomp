#include "m2c_prelude.h"
M2C_UNK ClearFrameBuffers__Fv();                    /* extern */
M2C_UNK FlushCache(M2C_UNK);                        /* extern */
M2C_UNK SyncVBlank__Fv();                           /* extern */
M2C_UNK sceGsResetGraph(M2C_UNK, M2C_UNK, M2C_UNK, M2C_UNK); /* extern */
M2C_UNK sceGsSwapDBuff(M2C_UNK *, s32);              /* extern */
M2C_UNK sceGsSetHalfOffset(M2C_UNK *, M2C_UNK, M2C_UNK, s16); /* extern */
extern s32 D_002BE594;
extern M2C_UNK D_002DEB80;
extern M2C_UNK D_002DEBE0;
extern M2C_UNK D_002DECD0;
extern s32 D_002DEDB0;
extern s32 D_002DEDB4;
extern s32 D_002DEDB8;
extern s32 D_002DEDBC;
extern s32 D_002DFB5C;

/** @sly2-annot addr=0x16F788
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/rendering.md#gs-0x16f2d0-0x1709a8
 * @end-sly2-annot */
void GS_Interrupt__Fi(void) {
    M2C_UNK *var_a0;
    s32 temp_a1;
    s32 temp_v1;

    D_002DFB5C = 1;
    ClearFrameBuffers__Fv();
    SyncVBlank__Fv();
    D_002DFB5C = 0;
    sceGsResetGraph(0, 1, 2, 1);
    temp_v1 = ((u32) *(u32 *)0x12001000 >> 0xD) & 1;
    D_002DEDBC = 0;
    D_002BE594 = 0;
    D_002DEDB8 = temp_v1;
    D_002DEDB4 = temp_v1 == 0;
    if (D_002DEDB0 & 1) {
        var_a0 = &D_002DEBE0;
    } else {
        var_a0 = &D_002DECD0;
    }
    sceGsSetHalfOffset(var_a0, 0x800, 0x800, (s16) D_002DEDB4);
    FlushCache(0);
    temp_a1 = D_002DEDB0 + 1;
    D_002DEDB0 = temp_a1;
    sceGsSwapDBuff(&D_002DEB80, temp_a1);
}
