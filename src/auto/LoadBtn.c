#include "m2c_prelude.h"
/** @sly2-annot addr=0x148738
 * Purpose: BTN owner = palo
 * Info: proposed name LoadBtn (T1); TU button
 * Notes: names_v2 C proposed SetSensorAlarm__FP6SENSORP5ALARM (0.638)
 * Confidence: mid (draft) [T1 0.55] - evidence: static naming pass; sources: r2b_rest
 * See: docs/reference/functions/gameplay-objects.md#button-0x148620-0x149a50
 * @end-sly2-annot */
void LoadBtn(s32 *arg0, s32 arg1) {
    *arg0 = arg1;
}
