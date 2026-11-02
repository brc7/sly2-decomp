#include "m2c_prelude.h"
s32 U16Read__18CBinaryInputStream();                /* extern */
M2C_UNK func_00117968(s32, s32, M2C_UNK);           /* extern */

/** @sly2-annot addr=0x118420
 * Purpose: Skips a U16-length-prefixed string (Read(U16Read(),NULL))
 * Info: proposed name CBinaryInputStream_SkipStringSw (T1); class CBinaryInputStream; TU bis
 * Notes: names_v2 B proposed Unknown1__18CBinaryInputStream (0.881) | level_parser CONFIRMED-parse: payload
 *   layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: mid (draft) [T1 0.60] - evidence: static naming pass; sources: r1_116918+parse-verified
 * See: docs/engine/core-library.md#7-key-functions, docs/engine/player.md#11-key-functions,
 *   docs/engine/world-sw.md#10-key-functions
 * @end-sly2-annot */
void CBinaryInputStream_SkipStringSw(s32 arg0) {
    func_00117968(arg0, U16Read__18CBinaryInputStream(), 0);
}
