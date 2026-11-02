#include "m2c_prelude.h"
M2C_UNK InitXfm();                            /* extern */
extern s128 D_002A2DA0;
extern s128 D_002A2DC0;

/** @sly2-annot addr=0x12E600
 * Purpose: vtable slot 7 (Init) of class cid 0x2C (XFM child, 0x140 B): InitXfm (0x231100) then field init
 * Info: proposed name InitCid44 (T2); class CID44 slot7; TU tu_12E600
 * Confidence: low (draft) [T2 0.55] - evidence: static analysis; sources: r1_116918
 * See: docs/engine/animation.md#6-source-files, docs/engine/animation.md#open-questions,
 *   docs/engine/camera.md#4-source-files
 * @end-sly2-annot */
void func_0012E600(void *arg0) {
    InitXfm();
    M2C_FIELD(arg0, s128 *, 0x100) = (s128) D_002A2DA0;
    M2C_FIELD(arg0, s128 *, 0x110) = (s128) D_002A2DC0;
}
