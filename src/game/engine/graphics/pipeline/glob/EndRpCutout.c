#include "m2c_prelude.h"
M2C_UNK EndRpGlob();                            /* extern */
M2C_UNK AddVifDirectFlushed(s32, M2C_UNK, M2C_UNK *);     /* extern */
extern M2C_UNK D_002DF720;

/** @sly2-annot addr=0x16E400
 * Purpose: Pass 11 end: EndRpGlob + DIRECT FBA_1=0 packet D_002DF720
 * Info: proposed name EndRpCutout (T1); TU glob
 * Notes: no usable identifier; purpose only
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r3_1539A8+rendering
 * See: docs/engine/rendering.md#4-the-26-kinds-and-12-passes-cstatic-tables-dumped-from-the-elf-roles-cstatic-unless-marked,
 *   docs/reference/functions/rendering.md#glob-0x167310-0x16f148
 * @end-sly2-annot */
void EndRpCutout(s32 arg0) {
    EndRpGlob();
    AddVifDirectFlushed(arg0, 2, &D_002DF720);
}
