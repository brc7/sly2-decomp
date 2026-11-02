#include "m2c_prelude.h"
M2C_UNK func_001B1BF0(void *);                      /* extern */
M2C_UNK LoadSoFromBrx();                            /* extern */
M2C_UNK func_002303A0(void *, void *, M2C_UNK);     /* extern */

/** @sly2-annot addr=0x1B0CA8
 * Purpose: CID 96 vt14: LoadSoFromBrx, set XF role of child x58, cache id, resolve linked objects
 * Info: proposed name LoadCid96FromBrx (T2); class CID96 slot14; TU tu_1B0B78
 * Notes: level_parser CONFIRMED-code (layout read from the matched function; class absent from samples)
 * Confidence: low (draft) [T2 0.65] - evidence: static analysis; sources: r5b_rest+parse-code
 * See: docs/formats/level-objects.md#5-per-class-payloads-vtable-slot-14,
 *   docs/reference/functions/gameplay-objects.md#tu_1b0b78-0x1b0b78-0x1b1db8
 * @end-sly2-annot */
void func_001B0CA8(void *arg0) {
    void *temp_v0;
    void *temp_v1;

    LoadSoFromBrx();
    func_002303A0(arg0, M2C_FIELD(arg0, void **, 0x58), 1);
    temp_v1 = M2C_FIELD(arg0, void **, 0x58);
    temp_v0 = temp_v1 + 0xF0;
    M2C_FIELD(temp_v0, s32 *, 8) = 0;
    M2C_FIELD(temp_v0, s32 *, 4) = 0;
    M2C_FIELD(temp_v1, s32 *, 0xF0) = 0;
    M2C_FIELD(arg0, s32 *, 0x1D4) = (s32) M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), s16 *, 0x98);
    func_001B1BF0(arg0);
}
