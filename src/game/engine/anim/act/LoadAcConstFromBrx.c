#include "m2c_prelude.h"
M2C_UNK Read__18CBinaryInputStreamiPv(s32, M2C_UNK, s32);           /* extern */

/** @sly2-annot addr=0x12B4F8
 * Purpose: Reads a fixed-size constant record (Read)
 * Info: proposed name LoadAcConstFromBrx (T1); class AcConst; TU act
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r1_116918+parse-verified
 * See: docs/engine/animation.md#7-key-functions,
 *   docs/formats/level-objects.md#7-animation-segment-loadasegfrombrx-0x12f2a8-cparse,
 *   docs/reference/functions/animation.md#act-0x12b3b0-0x12e600
 * @end-sly2-annot */
void LoadAcConstFromBrx(s32 arg0, s32 arg1) {
    Read__18CBinaryInputStreamiPv(arg1, 0xC, arg0 + 4);
}
