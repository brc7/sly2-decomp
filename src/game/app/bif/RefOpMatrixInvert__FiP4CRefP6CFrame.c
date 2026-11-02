#include "m2c_prelude.h"
void func_001291A8(s32, M2C_UNK *);              /* extern */
void func_00129EC8(M2C_UNK *);                   /* extern */
void func_0012A0B0(M2C_UNK *, M2C_UNK);          /* extern */
extern f32 D_002DDED0;
extern M2C_UNK D_002DDF40;
extern M2C_UNK D_003DD040;
extern M2C_UNK D_003DD490;
extern s32 D_003E0F9C;
extern s32 D_003E0FA0;
extern s32 D_003E0FA4;
extern M2C_UNK D_004520A8;

/** @sly2-annot addr=0x122E40
 * Purpose: Splice builtin "matrix-invert": 0x11C2F0
 * Info: proposed name RefOpMatrixInvert (T1); TU bif
 * Confidence: high [T1 0.93] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
s32 RefOpMatrixInvert__FiP4CRefP6CFrame(s32 arg0, s32 arg1, s32 arg2) {
    struct {
        M2C_UNK m0;
        M2C_UNK m4;
    } fr;
    func_0012A0B0((void *)&fr, 0x1B);
    func_0011C2F0(M2C_FIELD(arg2, s32 *, 4), fr.m4);
    func_001291A8(arg0, (void *)&fr);
    func_00129EC8((void *)&fr);
    return arg0;
}
