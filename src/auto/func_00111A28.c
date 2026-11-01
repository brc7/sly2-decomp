#include "m2c_prelude.h"
void *func_001AE4F8(M2C_UNK);                       /* extern */
M2C_UNK func_00256360(void *);                      /* extern */
extern M2C_UNK D_002C8940;

/** @sly2-annot addr=0x111A28
 * Purpose: pfnNew factory for CID67 (cid 67): allocates 0x1010 bytes (operator new 0x1AE4F8), calls ctor
 *   func_00256360
 * Info: proposed name NewBearcub (T1); TU crt0_splicewrap
 * Notes: class identity from whole_game WE names/object strings: NewCid67 -> NewBearcub
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: r0_thunks+table-verified(D_002BAAE8
 *   pfnNew/size 185/185; vt slot6 199/199)+whole_game(class identity)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
void *func_00111A28(void) {
    void *temp_v0;

    temp_v0 = func_001AE4F8(0x1010);
    func_00256360(temp_v0);
    M2C_FIELD(temp_v0, M2C_UNK **, 0xC) = &D_002C8940;
    return temp_v0;
}
