#include "m2c_prelude.h"
s32 FIsBasicDerivedFrom__FP5BASIC3CID(s32, M2C_UNK);                    /* extern */
s32 func_0020A298(s32, s32, s32);                   /* extern */
extern s32 D_002DD648;

/** @sly2-annot addr=0x20A378
 * Purpose: 0x20A298 with extra MURRAY/cid 0x3A/0x3D case
 * Info: proposed name FStepguardTargetValid2 (T2); TU stepguard
 * Notes: verifier PLAUSIBLE: Numeric-suffix placeholder. Returns 1 when target is MURRAY (cid 0x36) and guard
 *   is not cid 0x3A/0x3D and not D_002DD648, else FStepguardTargetValid. | T2 forced: verifier: placeholder
 *   name
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7b_guard+verdicts_followups
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
s32 func_0020A378(s32 arg0, s32 arg1, s32 arg2) {
    if ((FIsBasicDerivedFrom__FP5BASIC3CID(arg1, 0x36) != 0) && (FIsBasicDerivedFrom__FP5BASIC3CID(arg0, 0x3A) == 0) && (FIsBasicDerivedFrom__FP5BASIC3CID(arg0, 0x3D) == 0) && (arg0 != D_002DD648)) {
        return 1;
    }
    return func_0020A298(arg0, arg1, arg2);
}
