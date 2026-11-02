#include "m2c_prelude.h"
M2C_UNK CopyAb__FPvT0Ui(s32, s32, M2C_UNK);         /* extern */

/** @sly2-annot addr=0x1D4DE0
 * Purpose: CopyAb 0x9C bytes from +0x14
 * Info: proposed name CopyRobottankWeapon (T2); TU tu_1D1370
 * Notes: names_v2 C proposed GetAloLookAtTiltFunction__FP3ALOP3CLQ (0.438) | verifier PLAUSIBLE:
 *   CopyAb(dst+0x14, src+0x14, 0x9C); only caller CloneRcCar. 'Weapon' is not supported by the code; it is a
 *   clone sub-struct copy. | class identity from whole_game WE names/object strings: CopyRcCarWeapon ->
 *   CopyRobottankWeapon | T2 forced: verifier: role word not supported by the code
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources:
 *   r5b_rest+verdicts_followups+whole_game(class identity)
 * See: docs/engine/gameplay-objects.md#key-functions,
 *   docs/reference/functions/gameplay-objects.md#tu_1d1370-0x1d1370-0x1d5150
 * @end-sly2-annot */
void func_001D4DE0(s32 arg0, s32 arg1) {
    CopyAb__FPvT0Ui(arg0 + 0x14, arg1 + 0x14, 0x9C);
}
