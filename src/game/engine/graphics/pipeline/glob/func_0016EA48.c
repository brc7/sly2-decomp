#include "m2c_prelude.h"
M2C_UNK AddDmaCnt__4DMAS();                         /* extern */
M2C_UNK AddVifFlusha__4VIFS(s32);                   /* extern */
M2C_UNK EndDmaCnt__4DMAS(s32);                      /* extern */

/** @sly2-annot addr=0x16EA48
 * Purpose: Pass 1 end: FLUSHA
 * Info: proposed name EndRpCelBorder (T1); TU glob
 * Notes: no usable identifier; purpose only
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r3_1539A8+rendering
 * See: docs/engine/rendering.md#4-the-26-kinds-and-12-passes-cstatic-tables-dumped-from-the-elf-roles-cstatic-unless-marked,
 *   docs/reference/functions/rendering.md#glob-0x167310-0x16f148
 * @end-sly2-annot */
void func_0016EA48(s32 arg0) {
    AddDmaCnt__4DMAS();
    AddVifFlusha__4VIFS(arg0);
    EndDmaCnt__4DMAS(arg0);
}
