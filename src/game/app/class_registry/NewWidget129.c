#include "m2c_prelude.h"
void *func_001AE4F8(M2C_UNK);                       /* extern */
M2C_UNK WIDGET_ctor(void *);                      /* extern */
extern M2C_UNK D_002DAD90;

/** @sly2-annot addr=0x112C90
 * Purpose: pfnNew factory for WIDGET129 (cid 129): allocates 0x220 bytes (operator new 0x1AE4F8), calls ctor
 *   WIDGET_ctor
 * Info: proposed name NewWidget129 (T1); TU crt0_splicewrap
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: r0_thunks+table-verified(D_002BAAE8
 *   pfnNew/size 185/185; vt slot6 199/199)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
void *NewWidget129(void) {
    void *temp_v0;

    temp_v0 = func_001AE4F8(0x220);
    WIDGET_ctor(temp_v0);
    M2C_FIELD(temp_v0, M2C_UNK **, 0xC) = &D_002DAD90;
    return temp_v0;
}
