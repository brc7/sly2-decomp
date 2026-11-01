#include "m2c_prelude.h"
M2C_UNK func_001A64E8(void *, M2C_UNK, M2C_UNK *);  /* extern */
M2C_UNK func_001F31E0();                            /* extern */
void *func_001F7B98(void *);                        /* extern */
M2C_UNK func_002303A0(void *, s32, M2C_UNK);        /* extern */
extern M2C_UNK D_002C0D40;

/** @sly2-annot addr=0x1C4EF8
 * Purpose: DPRIZE::LoadFromBrx (vt14): LoadSoFromBrx + snip objects (0x1A64E8)
 * Info: proposed name LoadDprizeFromBrx (T1); class DPRIZE slot14; TU prize
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r5_1AE130+parse-verified
 * See: docs/engine/game-progression.md#8-key-functions,
 *   docs/formats/level-objects.md#5-per-class-payloads-vtable-slot-14,
 *   docs/reference/functions/game-progression.md#prize-0x1c4e50-0x1c8cc0
 * @end-sly2-annot */
void func_001C4EF8(void *arg0) {
    func_001F31E0();
    M2C_FIELD(func_001F7B98(arg0), s32 *, 0xF0) = 1;
    func_002303A0(arg0, M2C_FIELD(arg0, s32 *, 0x58), 0x15);
    func_002303A0(arg0, M2C_FIELD(arg0, s32 *, 0x5C), 0x15);
    func_001A64E8(arg0, 1, &D_002C0D40);
}
