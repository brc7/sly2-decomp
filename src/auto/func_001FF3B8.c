#include "m2c_prelude.h"
M2C_UNK InitGrabspot(s32);                         /* extern */
M2C_UNK InitXfm();                            /* extern */

/** @sly2-annot addr=0x1FF3B8
 * Purpose: CID 0x27 vt7 Init: XFM::Init (0x231100) then init sub-object at +0xE0 (0x182C70)
 * Info: proposed name InitCid39 (T2); class CID39 slot7; TU tu_1FF3B8
 * Notes: Cid27->Cid39 (CID 0x27=39, vtable)
 * Confidence: low (draft) [T2 0.60] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/engine/gameplay-objects.md#source-files, docs/engine/sound.md#5-source-files,
 *   docs/reference/functions/gameplay-objects.md#tu_1ff3b8-0x1ff3b8-0x1ff690
 * @end-sly2-annot */
void func_001FF3B8(s32 arg0) {
    InitXfm();
    InitGrabspot(arg0 + 0xE0);
}
