#include "m2c_prelude.h"
M2C_UNK AddDmaCnt__4DMAS();                         /* extern */
M2C_UNK AddVifFlusha__4VIFS(s32);                   /* extern */
M2C_UNK EndDmaCnt__4DMAS(s32);                      /* extern */

/** @sly2-annot addr=0x13A858
 * Purpose: Pass 2 end: FLUSHA
 * Info: proposed name EndRpBlip (T1); TU blip
 * Notes: superseded: FlushBlipVu1 (r2b_rest)
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r2b_rest+rendering
 * See: docs/engine/rendering.md#4-the-26-kinds-and-12-passes-cstatic-tables-dumped-from-the-elf-roles-cstatic-unless-marked,
 *   docs/reference/functions/particles-effects.md#blip-0x138f08-0x13a9e8
 * @end-sly2-annot */
void func_0013A858(s32 arg0) {
    AddDmaCnt__4DMAS();
    AddVifFlusha__4VIFS(arg0);
    EndDmaCnt__4DMAS(arg0);
}
