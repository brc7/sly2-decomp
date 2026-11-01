#include "m2c_prelude.h"
M2C_UNK func_001A6CA8(s32, M2C_UNK);                /* extern */
M2C_UNK func_00231100();                            /* extern */

/** @sly2-annot addr=0x12EB48
 * Purpose: slot 7 of class cid 0x2B (XFM child, 0x100 B): InitXfm + SetLoBusyk
 * Info: proposed name InitCid43 (T2); class CID43 slot7; TU amb
 * Confidence: low (draft) [T2 0.55] - evidence: static analysis; sources: r1_116918
 * See: docs/engine/animation.md#6-source-files, docs/engine/animation.md#open-questions,
 *   docs/engine/camera.md#4-source-files
 * @end-sly2-annot */
void func_0012EB48(s32 arg0) {
    func_00231100();
    func_001A6CA8(arg0, 0);
}
