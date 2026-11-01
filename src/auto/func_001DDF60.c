#include "m2c_prelude.h"
s32 func_0013E368(M2C_UNK *, M2C_UNK);              /* extern */
M2C_UNK strcpy(s32, s32);                           /* extern */
M2C_UNK strcpy1(s32, M2C_UNK *);                    /* extern */
extern M2C_UNK D_002A7AA8;
extern M2C_UNK D_003DD490;

/** @sly2-annot addr=0x1DDF60
 * Purpose: Text callback 0x5D7: string 0x40 plus a button-glyph suffix
 * Info: proposed name BuildTitleText5D7 (T2); TU screen
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void func_001DDF60(s32 arg0) {
    strcpy(arg0, func_0013E368(&D_003DD490, 0x40));
    strcpy1(arg0, &D_002A7AA8);
}
