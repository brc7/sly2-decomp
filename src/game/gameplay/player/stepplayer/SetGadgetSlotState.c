#include "m2c_prelude.h"
M2C_UNK CopyAb__FPvT0Ui(s32, s32, s32);             /* extern */
M2C_UNK Read__18CBinaryInputStreamiPv(s32, s32, M2C_UNK *);         /* extern */
M2C_UNK Skip__18CBinaryInputStreami(s32, s32);                    /* extern */
M2C_UNK ReadVector4__18CBinaryInputStreamP7VECTOR4(s32, void *);                 /* extern */
M2C_UNK Decref__4CRef(s32 *);                       /* extern */
s32 func_001AE318(void *, s32, s32, M2C_UNK, s32, s32); /* extern */
extern s128 D_002A2DD0;
extern M2C_UNK D_002A4BE0;
extern f32 D_002A4D18;
extern M2C_UNK D_002A4D50;
extern M2C_UNK D_002A4D58;
extern M2C_UNK D_002AC880;
extern M2C_UNK D_002BAAE8;
extern M2C_UNK D_002BBB60;
extern M2C_UNK D_002BC9F0;
extern f32 D_002BCC74;
extern f32 D_002BCC78;
extern f32 D_002BCC7C;
extern f32 D_002BCC80;
extern f32 D_002BCC84;
extern f32 D_002BCC88;
extern f32 D_002BCC8C;
extern f32 D_002BCC90;
extern f32 D_002BCC98;
extern f32 D_002BCC9C;
extern f32 D_002BCCA0;
extern f32 D_002BCCA4;
extern f32 D_002BCCA8;
extern f32 D_002BCCAC;
extern f32 D_002BCCB0;
extern f32 D_002BCCB4;
extern M2C_UNK D_002BCCB8;
extern M2C_UNK D_002BCCC8;
extern M2C_UNK D_002BCCD8;
extern M2C_UNK D_002BCCF0;
extern M2C_UNK D_002BCD00;
extern f32 D_002BD260;
extern f32 D_002BD264;
extern s32 D_002C3258;
extern M2C_UNK D_002CBE20;
extern M2C_UNK D_002CBE38;
extern M2C_UNK D_002DD590;
extern s32 D_002DD5BC;
extern f32 D_002DDED0;
extern f32 D_002DDED4;
extern M2C_UNK D_002DDF40;
extern M2C_UNK D_002DE2E0;
extern M2C_UNK D_002DEDC0;
extern M2C_UNK D_002DFC00;
extern s32 D_002E1E40;
extern s32 D_002E1E54;
extern s32 D_002F7900;
extern s32 D_003D4A54;
extern M2C_UNK D_003DA0F0;
extern M2C_UNK D_003DD040;
extern M2C_UNK D_003DD490;
extern s32 D_003E1444;
extern s32 D_003E1468;
extern M2C_UNK D_003E1C20;
extern s32 D_003E1C34;

/** @sly2-annot addr=0x141C08
 * Purpose: Change a gadget slot's state: leaving 1 calls vt102 (stop gadget); stamps tState; state 1/2 call
 *   vt100 StartGadget (2 = one-shot, returns to 0)
 * Info: proposed name SetGadgetSlotState (T1); TU stepplayer
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void SetGadgetSlotState(s32 arg0, s32 arg1, s32 arg2) {
    s32 temp_v0;
    void *temp_v0_2;
    void *temp_v0_3;
    void *temp_v0_4;

    temp_v0 = M2C_FIELD(arg1, s32 *, 4);
    if (temp_v0 != arg2) {
        if (temp_v0 == 1) {
            temp_v0_2 = M2C_FIELD(arg0, void **, 0xC);
            M2C_FIELD(temp_v0_2, void (**)(s32, s32), 0x334)(arg0 + M2C_FIELD(temp_v0_2, s16 *, 0x330), M2C_FIELD(arg1, s32 *, 0));
        }
        M2C_FIELD(arg1, s32 *, 4) = arg2;
        M2C_FIELD(arg1, f32 *, 8) = (f32) D_002DDED0;
        switch (arg2) {                             /* irregular */
        case 2:
            temp_v0_3 = M2C_FIELD(arg0, void **, 0xC);
            M2C_FIELD(temp_v0_3, void (**)(s32, s32), 0x324)(arg0 + M2C_FIELD(temp_v0_3, s16 *, 0x320), M2C_FIELD(arg1, s32 *, 0));
            M2C_FIELD(arg1, s32 *, 4) = 0;
            return;
        case 1:
            temp_v0_4 = M2C_FIELD(arg0, void **, 0xC);
            M2C_FIELD(temp_v0_4, void (**)(s32, s32), 0x324)(arg0 + M2C_FIELD(temp_v0_4, s16 *, 0x320), M2C_FIELD(arg1, s32 *, 0));
            break;
        }
    }
}
