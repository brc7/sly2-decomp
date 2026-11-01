#include "m2c_prelude.h"
M2C_UNK func_0019D4D0(void *, void *);              /* extern */

/** @sly2-annot addr=0x19CBF8
 * Purpose: Init the STEPPLAYER+0xF00 look/recon mode context: owner, state -1, requested kind -1, then its
 *   HUD (0x19D4D0)
 * Info: proposed name InitJtMode (T1); TU mode
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r4b_rest
 * See: docs/engine/player.md#4-stepplayer-tu-0x13ff70-0x144638-file-name-unknown-lilly-stepplayer_rawc,
 *   docs/engine/player.md#10-source-files, docs/reference/functions/player.md#mode-0x19cbf8-0x19de10
 * @end-sly2-annot */
void func_0019CBF8(void *arg0, s32 arg1) {
    M2C_FIELD(arg0, s32 *, 0) = arg1;
    M2C_FIELD(arg0, s32 *, 4) = -1;
    M2C_FIELD(arg0, s32 *, 0x20) = -1;
    func_0019D4D0(arg0 + 0x24, arg0);
}
