#include "m2c_prelude.h"
s32 func_001C4840(M2C_UNK *);                       /* extern */
M2C_UNK func_001DB520(s32, M2C_UNK);                /* extern */
extern s32 D_002DD5BC;
extern s32 D_002E1E40;
extern s32 D_002F7900;
extern M2C_UNK D_003DD490;

/** @sly2-annot addr=0x1DB490
 * Purpose: Open the gadget menu page for the current hero: 0 Sly (g_pjt), 1 Bentley (D_002DD5BC), 2 Murray
 *   (D_002F7900)
 * Info: proposed name OpenGadgetMenuForHero (T1); TU screen
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void func_001DB490(s32 arg0) {
    s32 temp_v0;

    temp_v0 = func_001C4840(&D_003DD490);
    if (temp_v0 != 0) {
        if (temp_v0 == D_002E1E40) {
            func_001DB520(arg0, 0);
            return;
        }
        if (temp_v0 == D_002DD5BC) {
            func_001DB520(arg0, 1);
            return;
        }
        if (temp_v0 == D_002F7900) {
            func_001DB520(arg0, 2);
        }
    }
}
