#include "m2c_prelude.h"
M2C_UNK **PdcbAddLo();                          /* extern */
extern M2C_UNK DeferLoRemoveCallback;

/** @sly2-annot addr=0x1A5D30
 * Purpose: Queue Remove of plo to run later (pushes callback 0x1A5D08 on plo deferred list)
 * Info: proposed name DeferLoRemove (T1); TU lo
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r4_17F090
 * See: docs/engine/object-model.md#3-creation-hierarchy-and-removal,
 *   docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518
 * @end-sly2-annot */
void DeferLoRemove(void) {
    *PdcbAddLo() = &DeferLoRemoveCallback;
}
