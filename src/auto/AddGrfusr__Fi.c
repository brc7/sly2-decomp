#include "m2c_prelude.h"
M2C_UNK UpdateGrfjoytFromGrfusr__Fv();                            /* extern */
extern s32 D_002DFBC4;

/** @sly2-annot addr=0x187078
 * Purpose: g_grfusr (D_002DFBC4)
 * Info: proposed name AddGrfusr (T1); TU joy
 * Confidence: high [T1 0.90] - evidence: static analysis; sources: r4b_rest+verdicts_A1+verdicts_followups
 * See: docs/engine/main-loop-startup.md#8-key-functions,
 *   docs/reference/functions/main-loop-startup.md#joy-0x187030-0x188488
 * @end-sly2-annot */
void AddGrfusr__Fi(s32 arg0) {
    D_002DFBC4 |= arg0;
    UpdateGrfjoytFromGrfusr__Fv();
}
