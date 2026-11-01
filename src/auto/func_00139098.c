#include "m2c_prelude.h"
M2C_UNK func_0011A220(M2C_UNK *, void *);           /* extern */
M2C_UNK func_0011A2E8(s32, void *);                 /* extern */
extern M2C_UNK D_002DEB00;

/** @sly2-annot addr=0x139098
 * Purpose: Move blip from its group list to the pending-free list
 * Info: proposed name RemoveBlip (T1); TU blip
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r2b_rest+verdicts_A1
 * See: docs/engine/particles-effects.md#6-key-functions,
 *   docs/reference/functions/particles-effects.md#blip-0x138f08-0x13a9e8
 * @end-sly2-annot */
void func_00139098(void *arg0) {
    func_0011A2E8(M2C_FIELD(arg0, s32 *, 0x108C) + 0x448, arg0);
    func_0011A220(&D_002DEB00, arg0);
}
