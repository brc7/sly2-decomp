#include "m2c_prelude.h"
M2C_UNK func_0016E1A8();                            /* extern */
M2C_UNK func_0016E318(s32, M2C_UNK, M2C_UNK *);     /* extern */
extern M2C_UNK D_002DF700;

/** @sly2-annot addr=0x16E3C8
 * Purpose: Pass 11 begin (kinds 4/6): BeginRpGlob + DIRECT FBA_1=1 packet D_002DF700
 * Info: proposed name BeginRpCutout (T1); TU glob
 * Notes: no usable identifier; purpose only
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r3_1539A8+rendering
 * See: docs/engine/rendering.md#4-the-26-kinds-and-12-passes-cstatic-tables-dumped-from-the-elf-roles-cstatic-unless-marked,
 *   docs/reference/functions/rendering.md#glob-0x167310-0x16f148
 * @end-sly2-annot */
void func_0016E3C8(s32 arg0) {
    func_0016E1A8();
    func_0016E318(arg0, 2, &D_002DF700);
}
