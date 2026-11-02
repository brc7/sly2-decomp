#include "m2c_prelude.h"
void Decref__4CRef(M2C_UNK *);                   /* extern */
extern M2C_UNK D_0027A10C;
extern M2C_UNK D_0027A1D8;
extern M2C_UNK D_002BAAE8;
extern f32 D_002DDED0;
extern M2C_UNK D_003DD490;
extern f32 *D_003E0E80;
extern M2C_UNK D_003E1144;
extern M2C_UNK D_004520A8;

/** @sly2-annot addr=0x132D28
 * Purpose: RefEnsureOption with CRef(LM*)
 * Info: proposed name EnsureLoSidebagLm (T1); TU basic
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r2b_rest
 * See: docs/engine/object-model.md#9-key-functions,
 *   docs/reference/functions/object-model.md#basic-0x132b00-0x1336f0
 * @end-sly2-annot */
void EnsureLoSidebagLm__FP2LO5OPTIDP2LM(s32 arg0, s32 arg1, s32 arg2) {
    struct {
        M2C_UNK m0;
        char pad0[0xC];
        M2C_UNK m10;
    } fr;
    CRef_ctor_LM((s32) (void *)&fr, arg2);
    RefEnsureOption__FP5BASICiP4CRef((s32) &fr.m10, arg0, arg1, (s32) (void *)&fr);
    Decref__4CRef(&fr.m10);
    Decref__4CRef((void *)&fr);
}
