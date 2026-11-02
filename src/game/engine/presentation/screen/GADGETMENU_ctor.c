#include "m2c_prelude.h"
M2C_UNK func_001DA728();                            /* extern */
M2C_UNK ClearGadgetMenu(void *);                      /* extern */
extern M2C_UNK D_002D8568;

/** @sly2-annot addr=0x1DAA08
 * Purpose: Gadget menu constructor (vtable 0x2D8568) for D_003DA0F0
 * Info: proposed name GADGETMENU_ctor (T1); TU screen
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void *GADGETMENU_ctor(void *arg0) {
    func_001DA728();
    M2C_FIELD(arg0, M2C_UNK **, 0xC) = &D_002D8568;
    ClearGadgetMenu(arg0);
    return arg0;
}
