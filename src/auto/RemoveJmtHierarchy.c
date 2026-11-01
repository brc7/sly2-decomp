#include "m2c_prelude.h"
M2C_UNK RemoveDlEntry__FP2DLPv(M2C_UNK *, s32);              /* extern */
M2C_UNK RemoveLoHierarchy();                            /* extern */
extern M2C_UNK D_003E1420;

/** @sly2-annot addr=0x19CA20
 * Purpose: JMT vt12: RemoveLoHierarchy then remove from g_dlJmt
 * Info: proposed name RemoveJmtHierarchy (T1); class CID40 slot12; TU air
 * Notes: names_v2 B proposed OnJmtRemove__FP3JMT (0.856) [verified CORRECT] | mangled dropped (does not match
 *   final name): OnJmtRemove__FP3JMT
 * Confidence: mid (draft) [T1 0.75] - evidence: static naming pass; sources:
 *   r4b_rest+verdicts_B+names_v2-signature(verified)
 * See: docs/engine/player.md#11-key-functions, docs/reference/functions/player.md#air-0x195950-0x19cbf8
 * @end-sly2-annot */
void RemoveJmtHierarchy(s32 arg0) {
    RemoveLoHierarchy();
    RemoveDlEntry__FP2DLPv(&D_003E1420, arg0);
}
