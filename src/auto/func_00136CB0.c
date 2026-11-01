#include "m2c_prelude.h"
extern M2C_UNK D_002A4AE8;

/** @sly2-annot addr=0x136CB0
 * Purpose: BENTLEY vt103: returns "bentley" (save-variable namespace used for hpCur/mpCur/apukCur)
 * Info: proposed name BENTLEY_PchzSaveVarRoot (T1); class BENTLEY slot103; TU bentley
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r2b_rest+verdicts_followups
 * See: docs/engine/player.md#11-key-functions, docs/reference/functions/player.md#bentley-0x134648-0x1378d8
 * @end-sly2-annot */
M2C_UNK *func_00136CB0(void) {
    return &D_002A4AE8;
}
