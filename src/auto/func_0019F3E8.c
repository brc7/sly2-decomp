#include "m2c_prelude.h"
/** @sly2-annot addr=0x19F3E8
 * Purpose: Init the STEPPLAYER+0x9E0 safe-place table: store owner
 * Info: proposed name InitJtPlaces (T1); TU places
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r4b_rest
 * See: docs/engine/player.md#4-stepplayer-tu-0x13ff70-0x144638-file-name-unknown-lilly-stepplayer_rawc,
 *   docs/engine/player.md#10-source-files, docs/reference/functions/player.md#places-0x19f3e8-0x1a0300
 * @end-sly2-annot */
void func_0019F3E8(s32 *arg0, s32 arg1) {
    *arg0 = arg1;
}
