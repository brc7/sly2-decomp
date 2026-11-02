#include "m2c_prelude.h"
extern s128 D_002A2DC0;
extern s128 D_002A2DD0;
extern f32 D_002A8280;
extern f32 D_002A8284;
extern M2C_UNK D_002CA280;
extern M2C_UNK D_002CA2C0;
extern s32 D_002DD2E0;
extern s32 D_002DD2E4;
extern s32 D_002DD2E8;
extern s32 D_002DD2EC;
extern s32 D_002DD2F0;
extern s32 D_002DD2F4;
extern M2C_UNK D_002DDED0;
extern M2C_UNK D_003DD020;
extern M2C_UNK D_003DD490;
extern s32 D_003E12EC;
extern void *D_003E145C;
extern M2C_UNK D_003E15CC;
extern M2C_UNK D_004520A8;
extern M2C_UNK D_004520D0;
extern M2C_UNK D_004520F8;
extern M2C_UNK D_00452120;
extern M2C_UNK D_00452148;
extern M2C_UNK D_00452170;

/** @sly2-annot addr=0x1F8740
 * Purpose: --cref; at 0 destroy and free to pool D_003E15CC
 * Info: proposed name DecrefXfh (T1); class CID148 slot2; TU so
 * Notes: verifier CORRECT: HANDLE::Decref -> XFH::Decref
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r6_1D5150+verdicts_tu_r4r6
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void DecrefXfh(s32 arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = M2C_FIELD(arg0, s32 *, 0x10) - 1;
    M2C_FIELD(arg0, s32 *, 0x10) = temp_v0;
    if (temp_v0 <= 0) {
        func_00256EE0(arg0, 0);
        func_002580A8(arg0 + 4, 0);
        UnsubscribeLoAll(arg0);
        func_0011CD98((s32) &D_003E15CC, arg0);
    }
}
