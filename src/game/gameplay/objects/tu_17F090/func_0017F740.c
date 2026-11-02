#include "m2c_prelude.h"
M2C_UNK func_0017F110();                            /* extern */
M2C_UNK func_0017FD08(s32, M2C_UNK);                /* extern */

/** @sly2-annot addr=0x17F740
 * Purpose: vt12: base then state change via SetCid48State
 * Info: proposed name RemoveHackshipHierarchy (T2); class HACKSHIP slot12; TU tu_17F090
 * Notes: CID48->CID72 (CID 0x48=72, vtable) | class identity from whole_game WE names/object strings:
 *   RemoveCid72Hierarchy -> RemoveHackshipHierarchy
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r4b_rest+whole_game(class identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
void func_0017F740(s32 arg0) {
    func_0017F110();
    func_0017FD08(arg0, -1);
}
