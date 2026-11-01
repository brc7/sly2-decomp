#include "m2c_prelude.h"
M2C_UNK func_00129EC8(M2C_UNK *);                   /* extern */
extern M2C_UNK D_0027A10C;
extern M2C_UNK D_0027A1D8;
extern M2C_UNK D_002BAAE8;
extern f32 D_002DDED0;
extern M2C_UNK D_003DD490;
extern f32 *D_003E0E80;
extern M2C_UNK D_003E1144;
extern M2C_UNK D_004520A8;

/** @sly2-annot addr=0x131990
 * Purpose: Toggles the ASEGA's hands-off flag (+0x58) with Increment/DecrementSwHandsOff
 * Info: proposed name SetAsegaHandsOff (T1); TU asega
 * Notes: names_v2 B proposed UFromEaErrorFunc__FP2EAf (0.856)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r1_116918
 * See: docs/engine/animation.md#7-key-functions,
 *   docs/reference/functions/animation.md#asega-0x1308a8-0x131f70
 * @end-sly2-annot */
void SetAsegaHandsOff(s32 arg0, s32 arg1) {
    if ((FAsegaRetracted(arg0) == 0) && (arg1 != M2C_FIELD(arg0, s32 *, 0x58))) {
        if (arg1 != 0) {
            IncrementSwHandsOff((s32) &D_003DD490, 3);
        } else {
            DecrementSwHandsOff((s32) &D_003DD490, 3);
        }
        M2C_FIELD(arg0, s32 *, 0x58) = arg1;
    }
}
