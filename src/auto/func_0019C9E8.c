#include "m2c_prelude.h"
M2C_UNK func_0011A220(M2C_UNK *, s32);              /* extern */
M2C_UNK func_001A5800();                            /* extern */
extern M2C_UNK D_003E1420;

/** @sly2-annot addr=0x19C9E8
 * Purpose: JMT vt11: AddLoHierarchy then append to g_dlJmt D_003E1420
 * Info: proposed name AddJmtHierarchy (T1); class CID40 slot11; TU air
 * Notes: names_v2 B proposed OnJmtAdd__FP3JMT (0.859)
 * Confidence: mid (draft) [T1 0.75] - evidence: static naming pass; sources: r4b_rest
 * See: docs/reference/functions/player.md#air-0x195950-0x19cbf8
 * @end-sly2-annot */
void func_0019C9E8(s32 arg0) {
    func_001A5800();
    func_0011A220(&D_003E1420, arg0);
}
