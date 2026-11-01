#include "m2c_prelude.h"
M2C_UNK SetLoBusyk(void *, M2C_UNK);             /* extern */
M2C_UNK InitXfm();                            /* extern */

/** @sly2-annot addr=0x21D278
 * Purpose: InitXfm; grftak=0x17E; sRadiusTarget=25.0; +0xF4=-1; SetLoBusyk(3)
 * Info: proposed name InitTarget (T1); class TARGET slot7; TU target
 * Notes: names_v2 C proposed StartupHide__Fv (0.438)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r8_21BB88+verdicts_tu_r7r8
 * See: docs/engine/animation.md#6-source-files, docs/engine/gameplay-objects.md#gameplay-object-classes,
 *   docs/engine/gameplay-objects.md#source-files
 * @end-sly2-annot */
void InitTarget(void *arg0) {
    InitXfm();
    SetLoBusyk(arg0, 3);
    M2C_FIELD(arg0, s32 *, 0xE8) = 0x17E;
    M2C_FIELD(arg0, f32 *, 0xEC) = 25.0f;
    M2C_FIELD(arg0, s32 *, 0xF4) = -1;
}
