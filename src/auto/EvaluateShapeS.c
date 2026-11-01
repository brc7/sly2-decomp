#include "m2c_prelude.h"
f32 UFromShapeS();                                /* extern */
M2C_UNK EvaluateShapeU(s32, s32, s32, f32);          /* extern */

void EvaluateShapeS(s32 arg0, s32 arg1, s32 arg2) {
    EvaluateShapeU(arg0, arg1, arg2, UFromShapeS());
}
