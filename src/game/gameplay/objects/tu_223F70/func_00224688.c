#include "m2c_prelude.h"
/** @sly2-annot addr=0x224688
 * Purpose: True if the gadget id (+0x15C) is 11 (Health Extractor)
 * Info: proposed name FCid100HealthExtractor (T2); class CID100; TU tu_223F70
 * Notes: v3: superseded FCid100State11 | runtime5a #17: +0x15C is the gadget id; v2 FCid100State11 | T2:
 *   placeholder token Cid100
 * Confidence: low (draft) [T2 0.65] - evidence: runtime (save-state RAM); sources: r8_21BB88+runtime5a
 * See: docs/engine/player.md#7-bentley-bentleycpp-0x134648,
 *   docs/reference/functions/gameplay-objects.md#tu_223f70-0x223f70-0x2259d8
 * @end-sly2-annot */
s32 func_00224688(void *arg0) {
    return M2C_FIELD(arg0, s32 *, 0x15C) == 0xB;
}
