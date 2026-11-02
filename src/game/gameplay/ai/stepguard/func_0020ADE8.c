#include "m2c_prelude.h"
M2C_UNK CloneLo();                            /* extern */
M2C_UNK func_00209590(s32);                         /* extern */

/** @sly2-annot addr=0x20ADE8
 * Purpose: cid 0x78 vt41: CloneLo + clone binding
 * Info: proposed name CloneCid120Attack (T2); class CID120 slot41; TU stepguard
 * Notes: Cid78->Cid120 (CID 0x78=120, vtable)
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_0020ADE8(s32 arg0) {
    CloneLo();
    func_00209590(arg0 + 0xB0);
}
