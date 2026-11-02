#include "m2c_prelude.h"
/* Warning: missing "jr $ra" in last block of func_0020D468 (initial). */

M2C_UNK CopyAb__FPvT0Ui(void *, s32, s32);          /* extern */
M2C_UNK Read__18CBinaryInputStreamiPv(s32, s32, s32);               /* extern */
M2C_UNK Skip__18CBinaryInputStreami(s32, s32);                    /* extern */
M2C_UNK SetDecompress__18CBinaryInputStreamiPUc(M2C_UNK *, M2C_UNK, M2C_UNK); /* extern */
M2C_UNK func_001291A8(s32 *, s32 *, s32, s32);      /* extern */
M2C_UNK Decref__4CRef(s32 *);                       /* extern */
s32 func_001AE318(s32 *, s32, s32, s128);           /* extern */
M2C_UNK sceWrite(s32, s32, s32);               /* extern */
extern s32 D_002A39E8;
extern M2C_UNK D_002A8910;
extern M2C_UNK D_002A8968;
extern M2C_UNK D_002A8970;
extern M2C_UNK D_002A8980;
extern M2C_UNK D_002A8990;
extern M2C_UNK D_002A8998;
extern M2C_UNK D_002A89A0;
extern M2C_UNK D_002A89A8;
extern M2C_UNK D_002A89B0;
extern M2C_UNK D_002A89C0;
extern M2C_UNK D_002A89D0;
extern M2C_UNK D_002A89E0;
extern M2C_UNK D_002A89F0;
extern M2C_UNK D_002A8A00;
extern M2C_UNK D_002A8A10;
extern M2C_UNK D_002A8A20;
extern M2C_UNK D_002A8A38;
extern M2C_UNK D_002A8A48;
extern M2C_UNK D_002A8A58;
extern M2C_UNK D_002A8A60;
extern M2C_UNK D_002A8A78;
extern M2C_UNK D_002A8A88;
extern M2C_UNK D_002BBB60;
extern f32 D_002C3DF0;
extern s128 D_002C3E00;
extern f32 D_002C3E50;
extern f32 D_002C3E54;
extern f32 D_002C3E58;
extern f32 D_002C3E60;
extern f32 D_002C3E64;
extern f32 D_002C3E68;
extern f32 D_002C3E6C;
extern s32 D_002C3E70;
extern f32 D_002C3E80;
extern M2C_UNK D_002C3E88;
extern M2C_UNK D_002C3EA0;
extern M2C_UNK D_002C3EB0;
extern M2C_UNK D_002DC018;
extern void *D_002DD5BC;
extern s32 D_002DD648;
extern f32 D_002DDED0;
extern M2C_UNK D_002DDF40;
extern s32 D_002DE570;
extern M2C_UNK D_002DEB30;
extern s32 D_002DFB80;
extern s32 D_002DFBC4;
extern M2C_UNK D_002DFC00;
extern M2C_UNK D_002E0CCC;
extern void *D_002E1E40;
extern s32 D_002E1E54;
extern M2C_UNK D_002E1E88;
extern M2C_UNK D_002F66B0;
extern void *D_002F7900;
extern M2C_UNK D_002F7980;
extern s32 D_003D4A54;
extern M2C_UNK D_003D4A60;
extern M2C_UNK D_003D9E70;
extern M2C_UNK D_003DD040;
extern s32 D_003DD48C;
extern M2C_UNK D_003DD490;
extern s32 D_003E1080;
extern s32 D_003E1C30;
extern s32 D_003E1C38;
extern s32 D_003E1C3C;
extern s32 D_003E1C40;
extern s32 D_003E1C44;
extern s32 D_003E1C48;
extern s32 D_003E1C4C;
extern s32 D_003E1C50;
extern s32 D_003E1C54;
extern s32 D_003E1C58;
extern s32 D_00443740;

/** @sly2-annot addr=0x20D178
 * Purpose: enabled = 0, state 0
 * Info: proposed name DisableStuckWatch (T2); TU tu_20CE28
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/guards-ai.md#tu_20ce28-0x20ce28-0x20d5d0
 * @end-sly2-annot */
void func_0020D178(s32 arg0, s32 arg1, s32 arg2) {
    M2C_FIELD(arg0, s32 *, 0xC) = 0;
    func_0020D108(arg0, 0, arg2);
}
