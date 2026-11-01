#include "m2c_prelude.h"
M2C_UNK func_001C3ED8();                            /* extern */
M2C_UNK func_001D7480(s32, M2C_UNK);                /* extern */

/** @sly2-annot addr=0x1D7418
 * Purpose: Calls PO base 0x1C3ED8 then switch on arg -> SetCharSelectState(0/1)
 * Info: proposed name HandleSafehouseMessage (T2); class SAFEHOUSE slot79; TU tu_1D7238
 * Notes: names_v2 C proposed PasegFindStepguard__FP9STEPGUARD3OID (0.438) | agent qualifier: | class identity
 *   from whole_game WE names/object strings: HandleCharSelectPoMessage -> HandleSafehouseMessage
 * Confidence: low (draft) [T2 0.20] - evidence: static naming pass; sources: r6_1D5150+whole_game(class
 *   identity)
 * See: docs/reference/functions/player.md#tu_1d7238-0x1d7238-0x1d7a30
 * @end-sly2-annot */
void func_001D7418(s32 arg0, s32 arg1) {
    func_001C3ED8();
    switch (arg1) {                                 /* irregular */
    case 0:
        func_001D7480(arg0, 0);
        return;
    case 1:
        func_001D7480(arg0, 1);
        break;
    }
}
