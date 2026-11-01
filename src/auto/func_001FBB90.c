#include "m2c_prelude.h"
M2C_UNK func_001FBBC8(s32, s32);                    /* extern */
s32 func_001FC0F0(M2C_UNK);                         /* extern */

/** @sly2-annot addr=0x1FBB90
 * Purpose: Allocate a BANKSND (0x70) from SNDMGR pool and construct it with sfxid; it starts on next
 *   UpdateSnd
 * Info: proposed name PsndStartSound (T1); TU sound
 * Notes: names_v2 B proposed SetPambVol__FP3AMBf (0.856)
 * Confidence: mid (draft) [T1 0.60] - evidence: static naming pass; sources: r7_1F8DD8
 * See: docs/engine/animation.md#7-key-functions, docs/engine/main-loop-startup.md#8-key-functions,
 *   docs/engine/overview.md#6-subsystem-map
 * @end-sly2-annot */
void func_001FBB90(s32 arg0) {
    func_001FBBC8(func_001FC0F0(0x70), arg0);
}
