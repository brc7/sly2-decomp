#include "m2c_prelude.h"
s32 PloFindSwObject__FP2SWi3OIDP2LO(M2C_UNK *, M2C_UNK, M2C_UNK, M2C_UNK); /* extern */
extern M2C_UNK D_003DD490;

/** @sly2-annot addr=0x1DA2E0
 * Purpose: Notice banner (global D_003DA0C0): find its widget 0x371
 * Info: proposed name PostNoticeLoad (T1); TU screen
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void func_001DA2E0(void *arg0) {
    M2C_FIELD(arg0, s32 *, 0x28) = PloFindSwObject__FP2SWi3OIDP2LO(&D_003DD490, 0x405, 0x371, 0);
}
