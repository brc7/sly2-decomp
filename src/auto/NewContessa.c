#include "m2c_prelude.h"
void *func_001AE4F8(M2C_UNK);                       /* extern */
M2C_UNK func_00256360(void *);                      /* extern */
extern M2C_UNK D_002CD0A0;

/** @sly2-annot addr=0x111968
 * Purpose: pfnNew factory for CID64 (cid 64): allocates 0x1010 bytes (operator new 0x1AE4F8), calls ctor
 *   func_00256360
 * Info: proposed name NewContessa (T1); TU crt0_splicewrap
 * Notes: class identity from whole_game WE names/object strings: NewCid64 -> NewContessa
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: r0_thunks+table-verified(D_002BAAE8
 *   pfnNew/size 185/185; vt slot6 199/199)+whole_game(class identity)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
void *NewContessa(void) {
    void *temp_v0;

    temp_v0 = func_001AE4F8(0x1010);
    func_00256360(temp_v0);
    M2C_FIELD(temp_v0, M2C_UNK **, 0xC) = &D_002CD0A0;
    return temp_v0;
}
