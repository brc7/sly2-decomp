#include "m2c_prelude.h"
M2C_UNK InitGrabspot(s32);                         /* extern */
M2C_UNK InitShape();                            /* extern */

/** @sly2-annot addr=0x1CB908
 * Purpose: RAIL::Init (vt7): InitShape + init GRABSPOT base at +0x190; +0x1C4=1, +0x1C0=1000
 * Info: proposed name InitRail (T1); class RAIL slot7; TU rail
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r5_1AE130
 * See: docs/engine/object-model.md#8-source-files, docs/engine/player.md#10-source-files,
 *   docs/reference/functions/player.md#rail-0x1cb908-0x1cbeb0
 * @end-sly2-annot */
void InitRail(void *arg0) {
    InitShape();
    InitGrabspot(arg0 + 0x190);
    M2C_FIELD(arg0, f32 *, 0x1C4) = 1.0f;
    M2C_FIELD(arg0, f32 *, 0x1C0) = 1000.0f;
}
