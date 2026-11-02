#include "m2c_prelude.h"
M2C_UNK ResetSlotheap(M2C_UNK *);                   /* extern */
M2C_UNK GcShutdownPairPool();                            /* extern */
M2C_UNK GcShutdownFramePool();                            /* extern */
M2C_UNK GcShutdownBindingPool();                            /* extern */
M2C_UNK GcShutdownProcPool();                            /* extern */
M2C_UNK GcShutdownSidebagPool();                            /* extern */
M2C_UNK GcShutdownPool6Pool();                            /* extern */
extern s32 D_002DD5B8;
extern M2C_UNK D_00452188;
extern M2C_UNK D_004521A0;
extern M2C_UNK D_004521B8;
extern M2C_UNK D_004521D0;
extern M2C_UNK D_004521E8;

/** @sly2-annot addr=0x12AE28
 * Purpose: Clears D_002DD5B8 and shuts down/reset the 6 splice splotheaps (0x257E88..0x257FC8) + 0x11CDB8
 * Info: proposed name ShutdownSplice (T1); TU spliceutils
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#spliceutils-0x12ae18-0x12b0e8,
 *   docs/reference/functions/splice-scripting.md#splicemap-0x1ff7e8-0x1ffcf8
 * @end-sly2-annot */
void ShutdownSplice(void) {
    D_002DD5B8 = 0;
    GcShutdownPairPool();
    GcShutdownFramePool();
    GcShutdownBindingPool();
    GcShutdownProcPool();
    GcShutdownSidebagPool();
    GcShutdownPool6Pool();
    ResetSlotheap(&D_00452188);
    ResetSlotheap(&D_004521A0);
    ResetSlotheap(&D_004521B8);
    ResetSlotheap(&D_004521D0);
    ResetSlotheap(&D_004521E8);
}
