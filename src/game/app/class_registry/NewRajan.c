#include "m2c_prelude.h"
void *func_001AE4F8(M2C_UNK);                       /* extern */
M2C_UNK STEPGUARD_ctor(void *);                      /* extern */
extern M2C_UNK D_002D7188;

/** @sly2-annot addr=0x1118A8
 * Purpose: pfnNew factory for CID61 (cid 61): allocates 0x1020 bytes (operator new 0x1AE4F8), calls ctor
 *   STEPGUARD_ctor
 * Info: proposed name NewRajan (T1); TU crt0_splicewrap
 * Notes: class identity from whole_game WE names/object strings: NewCid61 -> NewRajan
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: r0_thunks+table-verified(D_002BAAE8
 *   pfnNew/size 185/185; vt slot6 199/199)+whole_game(class identity)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
void *NewRajan(void) {
    void *temp_v0;

    temp_v0 = func_001AE4F8(0x1020);
    STEPGUARD_ctor(temp_v0);
    M2C_FIELD(temp_v0, M2C_UNK **, 0xC) = &D_002D7188;
    return temp_v0;
}
