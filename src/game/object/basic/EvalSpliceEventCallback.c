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

/** @sly2-annot addr=0x133000
 * Purpose: RefEval the call list in the object's frame; copy result to *pref if given
 * Info: proposed name EvalSpliceEventCallback (T1); TU basic
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/object-model.md#basic-0x132b00-0x1336f0
 * @end-sly2-annot */
void EvalSpliceEventCallback(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    struct {
        M2C_UNK m0;
    } fr;
    RefEval__FP5CPairP6CFrame((s32) (void *)&fr, arg1, arg2);
    if (arg3 != 0) {
        __as__4CRefRC4CRef(arg3, (s32) (void *)&fr);
    }
    Decref__4CRef((void *)&fr);
}
