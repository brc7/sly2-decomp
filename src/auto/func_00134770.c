#include "m2c_prelude.h"
M2C_UNK func_001A64E8(s32, M2C_UNK, M2C_UNK *);     /* extern */
M2C_UNK func_002039C0();                            /* extern */
extern M2C_UNK D_002BC390;

/** @sly2-annot addr=0x134770
 * Purpose: BENTLEY vt14: STEP LoadFromBrx base (0x2039C0) then 0x1A64E8(p, 0x76, D_002BC390) (load
 *   class-specific data table)
 * Info: proposed name LoadBentleyFromBrx (T1); class BENTLEY slot14; TU bentley
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r2b_rest+parse-verified
 * See: docs/engine/player.md#11-key-functions,
 *   docs/formats/level-objects.md#5-per-class-payloads-vtable-slot-14,
 *   docs/reference/functions/player.md#bentley-0x134648-0x1378d8
 * @end-sly2-annot */
void func_00134770(s32 arg0) {
    func_002039C0();
    func_001A64E8(arg0, 0x76, &D_002BC390);
}
