#include "m2c_prelude.h"
/** @sly2-annot addr=0x144638
 * Purpose: JTGOAL init: store owning STEPPLAYER at +0
 * Info: proposed name InitJtgoal (T1); TU jtgoal
 * Notes: names_v2 B proposed SetRobVehicle__FP3ROB3OID (0.873)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r2b_rest+verdicts_followups
 * See: docs/engine/player.md#4-stepplayer-tu-0x13ff70-0x144638-file-name-unknown-lilly-stepplayer_rawc,
 *   docs/engine/player.md#10-source-files, docs/engine/player.md#11-key-functions
 * @end-sly2-annot */
void InitJtgoal(s32 *arg0, s32 arg1) {
    *arg0 = arg1;
}
