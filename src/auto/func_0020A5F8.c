#include "m2c_prelude.h"
M2C_UNK func_0011A220(M2C_UNK *, void *);           /* extern */
extern M2C_UNK D_003E145C;

/** @sly2-annot addr=0x20A5F8
 * Purpose: Init a shared per-target engagement record (0xAC): target +0xC, 1.0, 2.0, -1s, append to
 *   D_003E145C
 * Info: proposed name InitTargetRecord (T2); TU stepguard
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void *func_0020A5F8(void *arg0, s32 arg1) {
    M2C_FIELD(arg0, s32 *, 0xC) = arg1;
    M2C_FIELD(arg0, f32 *, 0x14) = 1.0f;
    M2C_FIELD(arg0, f32 *, 0x18) = 2.0f;
    M2C_FIELD(arg0, s32 *, 0x10) = -1;
    M2C_FIELD(arg0, s32 *, 8) = -1;
    func_0011A220(&D_003E145C, arg0);
    return arg0;
}
