#include "m2c_prelude.h"
M2C_UNK func_00231100();                            /* extern */

/** @sly2-annot addr=0x19C9B8
 * Purpose: JMT vt7 Init: InitXfm then sRadius(+0xE8)=100
 * Info: proposed name InitJmt (T1); class CID40 slot7; TU air
 * Notes: names_v2 B proposed InitCnvo__FP4CNVO (0.856)
 * Confidence: mid (draft) [T1 0.75] - evidence: static naming pass; sources: r4b_rest
 * See: docs/reference/functions/player.md#air-0x195950-0x19cbf8
 * @end-sly2-annot */
void func_0019C9B8(void *arg0) {
    func_00231100();
    M2C_FIELD(arg0, f32 *, 0xE8) = 100.0f;
}
